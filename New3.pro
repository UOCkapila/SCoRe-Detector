#-------------------------------------------------
#
# Project created by QtCreator 2015-06-20T22:39:10
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = New3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    player.cpp \
    report.cpp \
    train.cpp

HEADERS  += mainwindow.h \
    player.h \
    comman.h \
    report.h \
    train.h

FORMS    += mainwindow.ui \
    report.ui \
    train.ui

INCLUDEPATH +="/usr/local/include"

LIBS += `pkg-config --libs opencv`

