#-------------------------------------------------
#
# Project created by QtCreator 2015-02-16T14:06:22
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET   = unsplash-retriever
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += link_pkgconfig
CONFIG   += c++11

PKGCONFIG += libxml-2.0

*-g++* {
    QMAKE_CXXFLAGS += -Wno-reorder
}


TEMPLATE = app

SOURCES += main.cpp \
    retriever.cpp \
    replymapper.cpp

HEADERS += \
    retriever.h \
    replymapper.h \
    retrieverphoto.h \
    ezETAProgressBar.hpp

