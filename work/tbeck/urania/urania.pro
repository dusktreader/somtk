# -------------------------------------------------
# Project created by QtCreator 2009-08-06T11:52:08
# -------------------------------------------------
TARGET = urania
TEMPLATE = app

CONFIG += qt xml

unix {
    message( "settin up build environment for unix" )

    INCLUDEPATH += \
        /usr/local/include

    LIBS += \
        -lopencv_core \
        -lopencv_ml \
        -lopencv_highgui

    CONFIG(debug) {
        LIBS += -efence
    }
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

    INCLUDEPATH += \
        C:/OpenCV/include \


    LIBS += \
        -lopencv_core230    \
        -lopencv_ml230      \
        -lopencv_highgui230 \


    CONFIG(debug) {
        LIBS += -efence
    }
    CONFIG(release) {
        LIBS += -fopenmp
    }
}

QMAKE_CXXFLAGS_RELEASE += -fopenmp -g
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g



INCLUDEPATH += \
    $$PWD/tools/ \

SOURCES += \
    tools/localassert.cpp \
    feature.cpp \
    histogram.cpp \
    hsom.cpp \
    imagesuspect.cpp \
    som.cpp \
    suspect.cpp \
    classifier.cpp \
    annclassifier.cpp \
    normalizer.cpp \
    sigmoidnormalizer.cpp \
    minmaxnormalizer.cpp

HEADERS += \
    tools/localassert.h \
    feature.h \
    hexgrid.hpp \
    histogram.h \
    hsom.h \
    suspect.h \
    imagesuspect.h \
    som.h \
    somerror.h \
    classifier.h \
    annclassifier.h \
    normalizer.h \
    sigmoidnormalizer.h \
    minmaxnormalizer.h

FORMS += mainwindow.ui \
    traindialog.ui
