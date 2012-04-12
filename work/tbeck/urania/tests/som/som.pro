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

HEADERS +=                                          \
    $$SOMTK_ROOT/som.h                             \
    $$SOMTK_ROOT/grids/grid.hpp                    \
    $$SOMTK_ROOT/grids/quadgrid.hpp                \
    $$SOMTK_ROOT/grids/hexgrid.hpp                 \
    $$SOMTK_ROOT/grids/fasthexgrid.hpp             \
    $$SOMTK_ROOT/grids/wraphexgrid.hpp             \
    $$SOMTK_ROOT/feature.h                         \
    $$SOMTK_ROOT/normalizers/normalizer.h          \
    $$SOMTK_ROOT/normalizers/minmaxnormalizer.h    \
    $$SOMTK_ROOT/normalizers/sigmoidnormalizer.h   \
    $$SOMTK_ROOT/normalizers/nullnormalizer.h      \
    $$SOMTK_ROOT/tools/utilities.hpp               \
    $$SOMTK_ROOT/tools/localassert.h               \
    $$SOMTK_ROOT/tools/randmaster.h                \

SOURCES +=                                          \
    $$SOMTK_ROOT/som.cpp                           \
    $$SOMTK_ROOT/feature.cpp                       \
    $$SOMTK_ROOT/normalizers/normalizer.cpp        \
    $$SOMTK_ROOT/normalizers/nullnormalizer.cpp    \
    $$SOMTK_ROOT/normalizers/minmaxnormalizer.cpp  \
    $$SOMTK_ROOT/normalizers/sigmoidnormalizer.cpp \
    $$SOMTK_ROOT/tools/utilities.cpp               \
    $$SOMTK_ROOT/tools/localassert.cpp             \
    $$SOMTK_ROOT/tools/randmaster.cpp              \
    tst_somtest.cpp                                 \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g
