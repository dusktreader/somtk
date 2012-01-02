#-------------------------------------------------
#
# Project created by QtCreator 2011-10-03T16:57:08
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = color-som-demo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/local/include ../tools ../urania
LIBS += -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_ml

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g

HEADERS += \
    ../urania/som.h \
    ../urania/feature.h \
    ../urania/colorfeature.h \
    ../tools/tools.hpp \
    ../tools/cvtypesplus.hpp \

SOURCES += \
    ../urania/som.cpp \
    ../urania/feature.cpp \
    ../urania/colorfeature.cpp \
    ../tools/tools.cpp \
   main.cpp
