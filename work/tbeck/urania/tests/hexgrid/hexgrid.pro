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

URANIA_ROOT = $$PWD/../..
INCLUDEPATH += $$URANIA_ROOT

HEADERS +=                                       \
    $$URANIA_ROOT/grids/grid.hpp                 \
    $$URANIA_ROOT/grids/hexgrid.hpp              \
    $$URANIA_ROOT/feature.h                      \
    $$URANIA_ROOT/tools/utilities.hpp            \
    $$URANIA_ROOT/tools/localassert.h            \
    $$URANIA_ROOT/tools/randmaster.h             \

SOURCES +=                                       \
    $$URANIA_ROOT/tools/utilities.cpp            \
    $$URANIA_ROOT/tools/localassert.cpp          \
    $$URANIA_ROOT/tools/randmaster.cpp           \
    tst_hexgridtest.cpp                          \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g
