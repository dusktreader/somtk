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

SOMTK_ROOT = $$PWD/../..
INCLUDEPATH += $$SOMTK_ROOT

HEADERS +=                                       \
    $$SOMTK_ROOT/grids/grid.hpp                 \
    $$SOMTK_ROOT/grids/hexgrid.hpp              \
    $$SOMTK_ROOT/feature.h                      \
    $$SOMTK_ROOT/tools/utilities.hpp            \
    $$SOMTK_ROOT/tools/localassert.h            \
    $$SOMTK_ROOT/tools/randmaster.h             \
    $$SOMTK_ROOT/errors/somerror.h              \

SOURCES +=                                       \
    $$SOMTK_ROOT/tools/utilities.cpp            \
    $$SOMTK_ROOT/tools/localassert.cpp          \
    $$SOMTK_ROOT/tools/randmaster.cpp           \
    tst_hexgridtest.cpp                          \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g
