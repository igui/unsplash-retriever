#ifndef RETRIEVER_H
#define RETRIEVER_H

#include <QObject>
#include <QtNetwork>
#include <libxml/HTMLparser.h>
#include "replymapper.h"
#include "retrieverphoto.h"
#include "ezETAProgressBar.hpp"

class Retriever : public QObject
{
    Q_OBJECT
private:
    static const QString baseUrl;
    static const int parallelRequests;
    static const quint64 magicResumeFile;

    // internal
    int pendingPages;
    int pendingPhotos;
    ReplyMapper *replyPageMapper;
    QNetworkAccessManager *pageAccessManager;
    QNetworkAccessManager *imageAccessManager;
    QMap<QString, RetrieverPhoto> targetPhotos;
    ez::ezETAProgressBar *progressBar;

    // options
    bool showProgress;
    bool overwrite;
    bool resume;
    QString destination;
public:
    explicit Retriever(QObject *parent, bool showProgress, bool overwrite, bool resume, QString destination);
public slots:
    void run();
private slots:
    void replyFinished(QNetworkReply *, int);
    void replyFinished(QNetworkReply *, RetrieverPhoto);
private:
    bool checkReply(QNetworkReply *reply, int &counter);
    void requestPage(int pageNumber);
    void downloadAllTargetPhotos();
    void requestPhoto(const RetrieverPhoto& photo);
    bool photoExists(const RetrieverPhoto& photo);
    void savePendingPhotos();
    void restorePendingPhotos();
    static htmlDocPtr parseReply(QNetworkReply *);
    static QList<RetrieverPhoto> getPhotos(QNetworkReply *, htmlDocPtr doc);
signals:
    void finished();
};

#endif // RETRIEVER_H
