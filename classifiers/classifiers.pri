!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    !exists( $$SOMTK_ROOT/opencv.pri ) {
        error( "opencv.pri not found.  Please provide opencv.pri in the $$SOMTK_ROOT folder. See opencv.pri.example" )
    }
    include( $$SOMTK_ROOT/opencv.pri )

    LIBS *= -lopencv_core -lopencv_ml

    HEADERS +=                \
        $$PWD/classifier.h    \
        $$PWD/annclassifier.h \

    SOURCES +=                  \
        $$PWD/classifier.cpp    \
        $$PWD/annclassifier.cpp \
}
