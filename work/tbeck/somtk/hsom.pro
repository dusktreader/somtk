#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T15:06:29
#
#-------------------------------------------------

QT += gui
QT += xml

TARGET = hsom
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOMTK_ROOT = $$PWD
message( somtk root set to $$SOMTK_ROOT )

INCLUDEPATH += $$SOMTK_ROOT

include( $$SOMTK_ROOT/soms/soms.pri )
include( $$SOMTK_ROOT/suspects/suspects.pri )
include( $$SOMTK_ROOT/normalizers/normalizers.pri )
include( $$SOMTK_ROOT/histograms/histograms.pri )
include( $$SOMTK_ROOT/classifiers/classifiers.pri )

HEADERS += \
    library.h \
    hsom.h \

SOURCES +=   \
    library.cpp \
    main.cpp \
    hsom.cpp \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

QMAKE_CXXFLAGS_RELEASE += -fopenmp
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g
