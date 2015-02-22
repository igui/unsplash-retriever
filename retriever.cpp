#include "retriever.h"
#include <libxml/xpath.h>
#include <libxml/uri.h>
#include <QRegExp>
#include <stdexcept>

const QString Retriever::baseUrl = "https://unsplash.com/filter?&page=";
const int Retriever::parallelRequests = 10;
const u_int64_t Retriever::magicResumeFile = 0xa00df070;

Retriever::Retriever(QObject *parent, bool showProgress, bool overwrite, bool resume, QString destination) :
    QObject(parent),
    pendingPages(0),
    pendingPhotos(0),
    replyPageMapper(new ReplyMapper(this)),
    pageAccessManager(new QNetworkAccessManager(this)),
    imageAccessManager(new QNetworkAccessManager(this)),
    progressBar(NULL),
    showProgress(showProgress),
    overwrite(overwrite),
    resume(resume),
    destination(destination)
{
    connect(pageAccessManager, SIGNAL(finished(QNetworkReply*)),
            replyPageMapper, SLOT(map(QNetworkReply *)));
    connect(replyPageMapper, SIGNAL(replyFinished(QNetworkReply*,int)),
            this, SLOT(replyFinished(QNetworkReply*,int)));
    connect(replyPageMapper, SIGNAL(replyFinished(QNetworkReply*,RetrieverPhoto)),
            this, SLOT(replyFinished(QNetworkReply*,RetrieverPhoto)));
}


void Retriever::run()
{
    if(showProgress)
    {
        if(resume){
            std::cout << "Resuming" << std::endl;
            restorePendingPhotos();
            int nPhotos = targetPhotos.keys().length();
            std::cout << "Downloading " << nPhotos << " photo(s)" << std::endl;
            progressBar = new ez::ezETAProgressBar(nPhotos);
            progressBar->start();
            for(auto photo: targetPhotos)
                requestPhoto(photo);
        } else {
            std::cout << "Parsing pages" << std::endl;
        }
    }

    for(int i = 1; i <= parallelRequests; ++i)
        requestPage(i);
}

void Retriever::requestPage(int pageNumber)
{
    auto url = QUrl(baseUrl + QString::number(pageNumber));
    replyPageMapper->map(url, pageNumber);
    pageAccessManager->get(QNetworkRequest(url));
    ++pendingPages;
}

void Retriever::requestPhoto(const RetrieverPhoto& photo)
{
    replyPageMapper->map(photo.url, photo);
    pageAccessManager->get(QNetworkRequest(photo.url));
    ++pendingPhotos;
}


bool Retriever::checkReply(QNetworkReply *reply, int &counter)
{
    --counter;

    if(reply->error())
    {
        qCritical("Error while getting %s: %s", qPrintable(reply->request().url().toString()), qPrintable(reply->errorString()));
        QCoreApplication::exit(1);
        return false;
    }
    return true;
}

void Retriever::replyFinished(QNetworkReply *reply, int pageNumber)
{
    if(!checkReply(reply, pendingPages))
        return;

    htmlDocPtr doc = NULL;

    try {
        doc = parseReply(reply);
        auto photos = getPhotos(reply, doc);

        bool foundNewPhotos = false;

        for(auto photo: photos){
            if(!photoExists(photo)){
                targetPhotos[photo.id] = photo;
                foundNewPhotos = true;
            }
        }

        if(foundNewPhotos)
            requestPage(pageNumber + parallelRequests);

        if(pendingPages == 0)
            downloadAllTargetPhotos();
    } catch(std::exception& ex)
    {
        if(doc)
           xmlFreeDoc(doc);
        qFatal("Error procesing response %s: %s", qPrintable(reply->request().url().toString()), ex.what());
        QCoreApplication::exit(2);
        return;
    }

    xmlFreeDoc(doc);
}

void Retriever::downloadAllTargetPhotos()
{
    if(targetPhotos.empty()){
        if(showProgress){
            std::cout << "No new photos found." << std::endl;
        }
        QCoreApplication::exit(1);
    } else {
        if(showProgress){
            int nPhotos = targetPhotos.values().length();
            std::cout << "Found " << nPhotos << " new photo(s)" << std::endl;
            progressBar = new ez::ezETAProgressBar(nPhotos);
            progressBar->start();
        }

        savePendingPhotos();

        for(auto photo: targetPhotos)
            requestPhoto(photo);
    }
}

bool Retriever::photoExists(const RetrieverPhoto &photo)
{
    if(overwrite)
        return false; // photos are always considered new if overwrite is set

    auto dir = QDir(destination);
    auto filterName = QString("%1.*").arg(photo.id);
    auto files = dir.entryList({ filterName }, QDir::Files | QDir::NoSymLinks);
    return !files.isEmpty();
}

htmlDocPtr Retriever::parseReply(QNetworkReply *reply)
{
    QByteArray replyData = reply->readAll();
    const char *url = reply->request().url().toString().toStdString().c_str();

    htmlParserCtxtPtr ctxt = NULL;
    htmlDocPtr doc = NULL;

    try
    {
        ctxt = htmlNewParserCtxt();
        if(!ctxt)
            throw std::logic_error("Couldn't create parser context");

        int options = HTML_PARSE_RECOVER | HTML_PARSE_NONET  | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING;
        doc = htmlCtxtReadMemory(ctxt, replyData.data(), replyData.length(), url, "UTF-8", options);
        if(!doc)
            throw std::logic_error("Couldn't parse response");
    }
    catch(std::exception& ex)
    {
        if(doc)
            xmlFreeDoc(doc);
        if(ctxt)
            xmlFreeParserCtxt(ctxt);
        throw ex;
    }

    xmlFreeParserCtxt(ctxt);
    return doc;
}


QList<RetrieverPhoto> Retriever::getPhotos(QNetworkReply *reply, htmlDocPtr doc)
{
    xmlXPathContextPtr xpathCtxt = NULL;
    xmlXPathObjectPtr xpathObj = NULL;

    try
    {
        xpathCtxt = xmlXPathNewContext(doc);
        if(!xpathCtxt)
            throw std::logic_error("Couldn't parse response");

        // TODO: INCORRECT XPath
        xpathObj = xmlXPathEvalExpression(BAD_CAST "//*[@class='photo']//a", xpathCtxt);
        if(!xpathObj)
            throw std::logic_error("Couldn't evaluate xpath expression");

        QList<RetrieverPhoto> res;

        if(!xpathObj->nodesetval)
            return res; // no results

        int nPhotos = xpathObj->nodesetval->nodeNr;
        for(int i = 0; i < nPhotos; ++i){
            xmlNodePtr nodePtr = xpathObj->nodesetval->nodeTab[i];
            if(nodePtr->type != XML_ELEMENT_NODE)
                continue;

            xmlChar *href = xmlGetProp(nodePtr, BAD_CAST "href");
            if(!href)
            {
                qWarning("Retriever::getPhotos: No href in a");
                continue;
            }

            QRegExp regexp("/photos/(.*)/download");
            QString photoId;
            if(regexp.indexIn(reinterpret_cast<char *>(href)) >= 0){
                photoId = regexp.cap(1);
            } else {
                photoId = QString(reinterpret_cast<char *>(href)).replace("/", "-");
                qWarning("Couldn't recognize photo id");
            }

            xmlChar *absolutehref = xmlBuildURI(href, BAD_CAST reply->request().url().toString().toStdString().c_str());

            RetrieverPhoto photo;
            photo.id = photoId;
            photo.url = QUrl(reinterpret_cast<char *>(absolutehref));
            res.append(photo);

            delete absolutehref;
        }

        return res;
    }
    catch(std::exception& ex)
    {
        if(xpathObj)
            xmlXPathFreeObject(xpathObj);
        if(xpathCtxt)
            xmlXPathFreeContext(xpathCtxt);
        throw ex;
    }
}

void Retriever::replyFinished(QNetworkReply *reply, RetrieverPhoto photo)
{
    if(!checkReply(reply, pendingPhotos))
        return;

    auto requestUrl = reply->request().url();
    auto redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    if(!redirectUrl.isEmpty() && redirectUrl != requestUrl){
        photo.url = redirectUrl;
        requestPhoto(photo);
        return;
    }
    auto suffix = QMimeDatabase().mimeTypeForData(reply).preferredSuffix();
    auto fileName = QDir(destination).filePath(QString("%1.%2").arg(photo.id, suffix));

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());
    file.close();

    if(showProgress && progressBar)
        ++(*progressBar);

    targetPhotos.remove(photo.id);
    savePendingPhotos();

    if(pendingPhotos == 0){
        emit finished();
    }
}

void Retriever::savePendingPhotos()
{
    QFile file(QDir(destination).filePath("downloaded.dat"));
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    out << (quint64) magicResumeFile;
    auto ids = targetPhotos.keys();
    out << (quint32) ids.length();

    for(auto id: ids){
        out << id.toStdString().c_str();
        out << targetPhotos[id].url.toString().toStdString().c_str();
    }

    file.close();
}

void Retriever::restorePendingPhotos()
{
    QFile file(QDir(destination).filePath("downloaded.dat"));
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    quint64 fileMagic;
    in >> fileMagic;
    if(fileMagic != magicResumeFile){
        qFatal("Invalid restore file (Magic doesn't match)");
        QCoreApplication::exit(1);
        return;
    }

    quint32 nPhotos;
    in >> nPhotos;

    if(in.status() != QDataStream::Status::Ok){
        qFatal("Invalid restore file (Couldn't read length')");
        QCoreApplication::exit(1);
        return;
    }

    while(nPhotos--){
        char *id;
        char *url;
        in >> id;
        in >> url;
        if(!id || !*id || !url || !*url)
        {
            qFatal("Error reading restore file");
            QCoreApplication::exit(1);
            return;
        }
        targetPhotos[id] = { id, QUrl(url) };
    }

    file.close();
}
