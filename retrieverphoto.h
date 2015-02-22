#ifndef RETRIEVERPHOTO_H
#define RETRIEVERPHOTO_H

#include <QtCore>

struct RetrieverPhoto {
    QString id;
    QUrl url;
    int tries;
};

#endif // RETRIEVERPHOTO_H
