#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T15:06:29
#
#-------------------------------------------------

QT       += testlib

QT       += gui

TARGET = tst_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOMTK_ROOT = $$PWD/../../..
INCLUDEPATH += $$SOMTK_ROOT

include ( $$SOMTK_ROOT/grids/grids.pri )

SOURCES +=  tst_wraphexgridtest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE   += -fopenmp
QMAKE_CXXFLAGS_DEBUG   += -g
