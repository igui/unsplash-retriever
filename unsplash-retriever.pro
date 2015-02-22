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
CONFIG   += c++11

# test Qt
lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5")

# libxml
win32 {
    QMAKE_CXXFLAGS += -I"$$(LIBXML2_PATH)\include"
    LIBS += -L$$(LIBXML2_PATH)\lib -llibxml2
} else {
    CONFIG   += link_pkgconfig
    PKGCONFIG += libxml-2.0
}

# Copy DLLs to target dir
win32{
    QMAKE_PRE_LINK =copy /Y $$(LIBXML2_PATH)\lib\libxml2.dll $(DESTDIR)

    # Qt dependencies
    CONFIG(debug, debug|release) {
        QMAKE_PRE_LINK += & copy $$(QTDIR)\bin\Qt5Networkd.dll $(DESTDIR)
        QMAKE_PRE_LINK += & copy $$(QTDIR)\bin\Qt5Cored.dll $(DESTDIR)
    }
    CONFIG(release, debug|release) {
        QMAKE_PRE_LINK += & copy $$(QTDIR)\bin\Qt5Network.dll $(DESTDIR)
        QMAKE_PRE_LINK += & copy $$(QTDIR)\bin\Qt5Core.dll $(DESTDIR)
    }

    # Unicode libraries (http://qt-project.org/wiki/Deploying-Windows-Applications)
    QMAKE_PRE_LINK += & xcopy /Y $$(QTDIR)\bin\icu*.dll $(DESTDIR)
}

# disable reorder warnings
*-g++* {
    QMAKE_CXXFLAGS += -Wno-reorder
}
# disable insecure uses of old functions (e.g. sprintf)
*msvc* {
    QMAKE_CXXFLAGS += -wd4996
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

DISTFILES += \
    README.md

