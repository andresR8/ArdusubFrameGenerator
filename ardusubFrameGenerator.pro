#-------------------------------------------------
#
# Project created by QtCreator 2017-01-16T14:30:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ardusubFrameGenerator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    motoritem.cpp

HEADERS  += mainwindow.h \
    motoritem.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
