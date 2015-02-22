#ifndef REPLYPAGEMAPPER_H
#define REPLYPAGEMAPPER_H

#include <QObject>
#include <QtNetwork>
#include "retrieverphoto.h"

class ReplyMapperMapping {
public:
    enum Type { NONE, PHOTO, INT };
private:
    Type m_type;
    RetrieverPhoto m_photo;
    int m_pageNumber;
public:
    ReplyMapperMapping();
    ReplyMapperMapping(const RetrieverPhoto& photo);
    ReplyMapperMapping(const int pageNumber);
    RetrieverPhoto photo() const;
    int pageNumber() const;
    Type type() const;
};

class ReplyMapper : public QObject
{
    Q_OBJECT
private:
    QMap<QString, ReplyMapperMapping> urlToMapped;
public:
    explicit ReplyMapper(QObject *parent = 0);
signals:
    void replyFinished(QNetworkReply *, int);
    void replyFinished(QNetworkReply *, RetrieverPhoto);
public slots:
    void map(const QUrl& url, int pageNumber);
    void map(const QUrl& url, const RetrieverPhoto& photo);
    void map(QNetworkReply *);
};

#endif // REPLYPAGEMAPPER_H
