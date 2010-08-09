# -------------------------------------------------
# Project created by QtCreator 2009-08-06T11:52:08
# -------------------------------------------------

TARGET = test
TEMPLATE = app

CONFIG += qt

INCLUDEPATH += /usr/local/include/opencv ../tools
LIBS += -L /usr/local/lib -lcv -lcxcore -lhighgui -lml

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g


SOURCES += \
    ../tools/tools.cpp \
    ../tools/cvtools.cpp \
    testmain.cpp \
    hexgridtest.cpp \
    feature.cpp \
    featuretest.cpp \
    somhistogram.cpp \
    somhistogramtest.cpp \
    suspect.cpp \
    suspecttest.cpp \
    hsom.cpp \
    colorfeature.cpp \
    colorhsom.cpp \
    colorsuspect.cpp

HEADERS += \
    ../tools/tools.hpp \
    ../tools/cvtools.hpp \
    ../tools/cvtypesplus.hpp \
    unittest.hpp \
    hexgrid.hpp \
    hexgridtest.h \
    feature.h \
    featuretest.h \
    somhistogram.h \
    somhistogramtest.h \
    suspect.h \
    suspecttest.h \
    hsom.h \
    colorfeature.h \
    colorhsom.h \
    colorsuspect.h

FORMS +=
