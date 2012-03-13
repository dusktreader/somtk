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

HEADERS += \
    $$URANIA_ROOT/som.h  \
    $$URANIA_ROOT/feature.h \
    $$URANIA_ROOT/normalizer.h \
    $$URANIA_ROOT/tools/utilities.hpp \
    $$URANIA_ROOT/tools/localassert.h \
    $$URANIA_ROOT/tools/randmaster.h \
    nullnormalizer.h

SOURCES += \
    $$URANIA_ROOT/som.cpp  \
    $$URANIA_ROOT/feature.cpp \
    $$URANIA_ROOT/normalizer.cpp \
    $$URANIA_ROOT/tools/utilities.cpp \
    $$URANIA_ROOT/tools/localassert.cpp \
    $$URANIA_ROOT/tools/randmaster.cpp \
    nullnormalizer.cpp \
    tst_somtest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

#QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
#QMAKE_LFLAGS_RELEASE += -fopenmp
#QMAKE_CXXFLAGS_DEBUG += -g
