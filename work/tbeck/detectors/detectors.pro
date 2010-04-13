QT -= \
    core \
    gui \


TARGET = detectors
target.path = /usr/local
INSTALLS += target

TEMPLATE = lib

DEFINES += DETECTORS_LIBRARY

INCLUDEPATH += \
    /usr/local/include/opencv \
    ../tools \

LIBPATH += /usr/local/lib

LIBS += \
    -lcv \
    -lcxcore \
    -lhighgui \
    -lml \
    -lgomp \

SOURCES += \
    detector.cpp \
    fasthoughdetector.cpp \
    tophatdetector.cpp \
    cascadedetector.cpp \
    stardetector.cpp \

HEADERS += detector.h \
    fasthoughdetector.h \
    tophatdetector.h \
    cascadedetector.h \
    stardetector.h \
    ../tools/cvtypesplus.hpp \

QMAKE_CXXFLAGS_RELEASE += -fopenmp
QMAKE_LFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -g

QMAKE_CLEAN += Makefile* libdetectors.so*
