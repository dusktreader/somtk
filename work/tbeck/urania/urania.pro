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

#TODO:  Add .pri files for subdirectories.  Use WAS approach

SOURCES += \
    hsom.cpp \
    som.cpp \
    feature.cpp \
    histogram.cpp \
    classifiers/classifier.cpp \
    classifiers/annclassifier.cpp \
    normalizers/normalizer.cpp \
    normalizers/sigmoidnormalizer.cpp \
    normalizers/minmaxnormalizer.cpp \
    normalizers/nullnormalizer.cpp\
    suspects/imagesuspect.cpp \
    suspects/suspect.cpp \
    tools/localassert.cpp \

HEADERS += \
    hsom.h \
    som.h \
    feature.h \
    hexgrid.hpp \
    histogram.h \
    classifiers/classifier.h \
    errors/somerror.h \
    classifiers/annclassifier.h \
    normalizers/normalizer.h \
    normalizers/sigmoidnormalizer.h \
    normalizers/minmaxnormalizer.h \
    normalizers/nullnormalizer.h \
    suspects/suspect.h \
    suspects/imagesuspect.h \
    tools/localassert.h \

FORMS += mainwindow.ui \
    traindialog.ui
