#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T15:06:29
#
#-------------------------------------------------

QT       += gui

TARGET = tst_somtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOMTK_ROOT = $$PWD
INCLUDEPATH += $$SOMTK_ROOT

include( $$SOMTK_ROOT/soms/soms.pri )
include( $$SOMTK_ROOT/suspects/suspects.pri )
include( $$SOMTK_ROOT/normalizers/nomralizers.pri )
include( $$SOMTK_ROOT/classifiers/classifiers.pri )
include( $$SOMTK_ROOT/histograms/histograms.pri )

HEADERS += \
    hsom.h \

SOURCES +=   \
    hsom.cpp \
    main.cpp \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g
