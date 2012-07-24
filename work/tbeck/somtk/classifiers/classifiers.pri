!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    !exists( $$PWD/opencv.pri ) {
        error( "opencv.pri not found.  Please provide opencv.pri in the $$PWD folder. See opencv.pri.example" )
    }
    include( $$PWD/opencv.pri )

    INCLUDEPATH += $$OPENCV_INCLUDEPATH
    LIBS += -L$$OPENCV_LIBRARYPATH -lopencv_core -lopencv_ml

    HEADERS +=                \
        $$PWD/classifier.h    \
        $$PWD/annclassifier.h \

    SOURCES +=                  \
        $$PWD/classifier.cpp    \
        $$PWD/annclassifier.cpp \
}
