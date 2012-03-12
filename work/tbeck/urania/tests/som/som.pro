#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T15:06:29
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_somtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

URANIA_ROOT = $$PWD/../..
INCLUDEPATH += $$URANIA_ROOT

HEADERS += \
    som.h  \
    feature.h \
    suspect.h \


SOURCES += tst_somtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
