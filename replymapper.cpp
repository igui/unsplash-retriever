#include "replymapper.h"

ReplyMapper::ReplyMapper(QObject *parent) :
    QObject(parent)
{
}

void ReplyMapper::map(QNetworkReply *reply)
{
    auto url = reply->request().url().toString();
    auto value = urlToMapped.value(url);

    switch (value.type()) {
    case ReplyMapperMapping::INT:
        emit replyFinished(reply, value.pageNumber());
        break;
    case ReplyMapperMapping::PHOTO:
        emit replyFinished(reply, value.photo());
        break;
    default:
        qWarning("No mapping done for page %s", qPrintable(url));
        break;
    }
}

void ReplyMapper::map(const QUrl &url, int pageNumber)
{
    urlToMapped[url.toString()] = pageNumber;
}


void ReplyMapper::map(const QUrl& url, const RetrieverPhoto& photo)
{
    urlToMapped[url.toString()] = photo;
}


ReplyMapperMapping::ReplyMapperMapping():
    m_type(Type::NONE){
}

ReplyMapperMapping::ReplyMapperMapping(const RetrieverPhoto& photo):
    m_type(Type::PHOTO)
{
    m_photo = photo;
}

ReplyMapperMapping::ReplyMapperMapping(const int pageNumber):
    m_type(Type::INT){
    m_pageNumber = pageNumber;
}

RetrieverPhoto ReplyMapperMapping::photo() const
{
    if(m_type == Type::PHOTO)
        return m_photo;
    else
        return RetrieverPhoto();
}

int ReplyMapperMapping::pageNumber() const
{
    if(m_type == Type::INT)
        return m_pageNumber;
    else
        return -1;
}

ReplyMapperMapping::Type ReplyMapperMapping::type() const
{
    return m_type;
}
