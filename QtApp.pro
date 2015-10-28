#-------------------------------------------------
#
# Project created by QtCreator 2015-10-17T18:23:34
#
#-------------------------------------------------

QT       += core gui webkit network webkitwidgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    authwindow.cpp \
    audioget.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    authwindow.h \
    audioget.h \
    about.h

FORMS    += mainwindow.ui \
    authwindow.ui \
    about.ui

CONFIG +=c++14

RESOURCES += \
    player_resources.qrc
