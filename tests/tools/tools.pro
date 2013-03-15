#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T15:06:29
#
#-------------------------------------------------

QT       += testlib

QT       += gui

TARGET = tst_toolstest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOMTK_ROOT = $$PWD/../..
INCLUDEPATH += $$SOMTK_ROOT

!exists( $$SOMTK_ROOT/opencv.pri ) {
    error( "opencv.pri not found.  Please provide opencv.pri in the $$SOMTK_ROOT folder. See opencv.pri.example" )
}
include( $$SOMTK_ROOT/opencv.pri )
LIBS *= -lopencv_core -lopencv_imgproc

include( $$SOMTK_ROOT/tools/cvq/cvq.pri )

SOURCES +=  tst_toolstest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE   += -fopenmp
QMAKE_CXXFLAGS_DEBUG   += -g
