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

URANIA_ROOT = $$PWD/../..
INCLUDEPATH += $$URANIA_ROOT

HEADERS +=                                       \
    $$URANIA_ROOT/som.h                          \
    $$URANIA_ROOT/grids/grid.hpp                 \
    $$URANIA_ROOT/grids/quadgrid.hpp             \
    $$URANIA_ROOT/grids/hexgrid.hpp              \
    $$URANIA_ROOT/grids/fasthexgrid.hpp          \
    $$URANIA_ROOT/grids/wraphexgrid.hpp          \
    $$URANIA_ROOT/feature.h                      \
    $$URANIA_ROOT/normalizers/normalizer.h       \
    $$URANIA_ROOT/normalizers/nullnormalizer.h   \
    $$URANIA_ROOT/tools/utilities.hpp            \
    $$URANIA_ROOT/tools/localassert.h            \
    $$URANIA_ROOT/tools/randmaster.h             \

SOURCES +=                                       \
    $$URANIA_ROOT/som.cpp                        \
    $$URANIA_ROOT/feature.cpp                    \
    $$URANIA_ROOT/normalizers/normalizer.cpp     \
    $$URANIA_ROOT/normalizers/nullnormalizer.cpp \
    $$URANIA_ROOT/tools/utilities.cpp            \
    $$URANIA_ROOT/tools/localassert.cpp          \
    $$URANIA_ROOT/tools/randmaster.cpp           \
    tst_somtest.cpp                              \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g
