#-------------------------------------------------
#
# Project created by QtCreator 2017-09-16T16:54:39
#
#-------------------------------------------------

QT       += core gui dbus network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Konek
TEMPLATE = app

contains(CONFIG, EMBED_LIBCONNMANQT) {
    include(lib/libconnman-qt/libconnman-qt.pri)
    DEFINES += EMBED_LIBCONNMANQT
} else {
    LIBS += -lconnman-qt5
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


SOURCES += \
        main.cpp \
        maindialog.cpp

HEADERS += \
        maindialog.h

FORMS += \
        maindialog.ui
