#-------------------------------------------------
#
# Project created by QtCreator 2010-04-13T11:33:19
#
#-------------------------------------------------

QT       += core gui

TARGET = finder
TEMPLATE = app


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../tools/tools.cpp \
    ../tools/qtools.cpp \

HEADERS  += \
    mainwindow.h \
    ../tools/tools.h \
    ../tools/qtools.h \

FORMS    +=  \
    mainwindow.ui

INCLUDEPATH += \
    /usr/local/include/opencv \
    ../tools \
    ../tools/MegaSlider \
    ../tools/ImageView \
    ../detectors \

LIBPATH += \
    /usr/local/lib \
    ../detectors \
    ../tools/MegaSlider \
    ../tools/ImageView \

LIBS += \
    -lcv \
    -lcxcore \
    -lhighgui \
    -lml \
    -lgomp \
    -lmegasliderplugin \
    -limageviewplugin \
    -ldetectors \




QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE += -fopenmp -g
