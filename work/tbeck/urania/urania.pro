# -------------------------------------------------
# Project created by QtCreator 2009-08-06T11:52:08
# -------------------------------------------------
TARGET = urania
TEMPLATE = app

CONFIG += qt

INCLUDEPATH += /usr/local/include/opencv ../tools
LIBS += -L /usr/local/lib -lcv -lcxcore -lhighgui -lml

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g


SOURCES += feature.cpp \
    imagehsom.cpp \
    local.cpp \
    hsom.cpp \
    somhistogram.cpp \
    qmain.cpp \
    mainwindow.cpp \
    qimagehsom.cpp \
    qlocal.cpp \
    suspect.cpp \
    imagesuspect.cpp \
    hufeature.cpp \
    traindialog.cpp \
    ../tools/tools.cpp \
    ../tools/cvtools.cpp \

HEADERS += feature.h \
    hexgrid.hpp \
    imagehsom.h \
    local.h \
    hsom.h \
    somhistogram.h \
    mainwindow.h \
    qimagehsom.h \
    qlocal.h \
    suspect.h \
    imagesuspect.h \
    hufeature.h \
    traindialog.h \
    ../tools/tools.h \
    ../tools/cvtools.h \
    ../tools/cvtypesplus.hpp \

FORMS += mainwindow.ui \
    traindialog.ui

