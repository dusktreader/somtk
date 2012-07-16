#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T15:06:29
#
#-------------------------------------------------

QT       += testlib

QT       += guid

TARGET = tst_histogramtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOMTK_ROOT = $$PWD/../..
INCLUDEPATH += $$SOMTK_ROOT

include( $$SOMTK_ROOT/histograms/histograms.pri )
include( $$SOMTK_ROOT/grids/grids.pri )

HEADERS +=                             \
    $$SOMTK_ROOT/tools/localassert.h   \

SOURCES +=                             \
    $$SOMTK_ROOT/tools/localassert.cpp \
    tst_histogramtest.cpp                    \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g
