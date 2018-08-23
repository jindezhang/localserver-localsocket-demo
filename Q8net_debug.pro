#-------------------------------------------------
#
# Project created by QtCreator 2018-08-09T08:43:49
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Q8net_debug
TEMPLATE = app

RC_ICONS = cat.ico
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    localserver_.cpp \
    localclinet_.cpp \
    tcpserver.cpp \
    tcpclient.cpp

HEADERS += \
        mainwindow.h \
    localserver_.h \
    localclinet_.h \
    tcpserver.h \
    tcpclient.h

FORMS += \
        mainwindow.ui
