#-------------------------------------------------
#
# Project created by QtCreator 2015-05-23T20:11:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dsse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    avltree.cpp \
    crypto.cpp \
    databace.cpp \
    dsse.cpp \
    wrapper.cpp \
    pair.cpp \
    list.cpp \
    threadfcrypto.cpp \
    threadsetup.cpp

HEADERS  += mainwindow.h \
    avltree.h \
    crypto.h \
    databace.h \
    dsse.h \
    global.h \
    list.h \
    pair.h \
    wrapper.h \
    threadfcrypto.h \
    threadsetup.h

FORMS    += mainwindow.ui
LIBS += ../libtomcrypt/libtomcrypt.a
INCLUDEPATH +=../libtomcrypt/src/headers
