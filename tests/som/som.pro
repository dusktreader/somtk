#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T15:06:29
#
#-------------------------------------------------

QT       += testlib

QT       += gui

TARGET = tst_somtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOMTK_ROOT = $$PWD/../..
INCLUDEPATH += $$SOMTK_ROOT
DEPENDPATH *= $${INCLUDEPATH}

include( $$SOMTK_ROOT/soms/soms.pri )
include( $$SOMTK_ROOT/tools/randmaster/randmaster.pri )


SOURCES +=                             \
    tst_somtest.cpp                    \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g
