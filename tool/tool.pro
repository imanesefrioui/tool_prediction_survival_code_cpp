#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T18:56:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    mythread.h

FORMS    += mainwindow.ui

RESOURCES += \
    ressources.qrc
