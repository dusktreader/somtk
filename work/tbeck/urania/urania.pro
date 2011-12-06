# -------------------------------------------------
# Project created by QtCreator 2009-08-06T11:52:08
# -------------------------------------------------
TARGET = urania
TEMPLATE = app

CONFIG += qt

INCLUDEPATH += /usr/local/include ../tools
LIBS += -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_ml

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g


SOURCES += \
    colorhsom.cpp \
    feature.cpp \
    histogram.cpp \
    hsom.cpp \
    hufeature.cpp \
    imagehsom.cpp \
    imagesuspect.cpp \
    mainwindow.cpp \
    qimagehsom.cpp \
    qmain.cpp \
    som.cpp \
    suspect.cpp \
    traindialog.cpp \
    ../tools/tools.cpp \

HEADERS += feature.h \
    hexgrid.hpp \
    histogram.h \
    hsom.h \
    hufeature.h \
    imagehsom.h \
    colorhsom.h \
    mainwindow.h \
    qimagehsom.h \
    suspect.h \
    imagesuspect.h \
    traindialog.h \
    som.h \
    ../tools/tools.hpp \
    ../tools/cvtools.hpp \
    ../tools/cvtypesplus.hpp \

FORMS += mainwindow.ui \
    traindialog.ui
