#-------------------------------------------------
#
# Project created by QtCreator 2012-02-27T13:56:44
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_somtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


unix {
    message( "settin up build environment for unix" )

    CONFIG(release) {
        LIBS += -fopenmp
    }
}

macx {
    message( "settin up build environment for mac" )
    error( "urania is not prepared to build for the mac platform" )
}

win32 {
    message( "settin up build environment for windows" )

    CONFIG(release) {
        LIBS += -fopenmp
    }
}

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g

INCLUDEPATH += \
    $$URANIA_ROOT \

HEADERS += \
    feature.h \
    som.h \
    minmaxnormalizer.h \
    suspect.h \

SOURCES += tst_somtest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
