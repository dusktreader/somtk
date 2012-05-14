#-------------------------------------------------
#
# Project created by QtCreator 2012-05-14T12:33:48
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_sigmoidnormalizertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOMTK_ROOT = $$PWD/../../..
INCLUDEPATH += $$SOMTK_ROOT

include( $$SOMTK_ROOT/normalizers/normalizers.pri )

HEADERS +=                             \
    $$SOMTK_ROOT/tools/randmaster.h    \
    $$SOMTK_ROOT/tools/localassert.h   \

SOURCES +=                             \
    $$SOMTK_ROOT/tools/randmaster.cpp  \
    $$SOMTK_ROOT/tools/localassert.cpp \
    tst_sigmoidnormalizertest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
