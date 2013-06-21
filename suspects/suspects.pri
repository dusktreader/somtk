!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    !exists( $$SOMTK_ROOT/opencv.pri ) {
        error( "opencv.pri not found.  Please provide opencv.pri in the $$SOMTK_ROOT folder. See opencv.pri.example" )
    }
    include( $$SOMTK_ROOT/opencv.pri )
    LIBS *= -lopencv_core -lopencv_imgproc

    include( $$SOMTK_ROOT/features/features.pri )
    include( $$SOMTK_ROOT/histograms/histograms.pri )
    include( $$SOMTK_ROOT/tools/cvq/cvq.pri )

    HEADERS +=                    \
        $$PWD/suspect.h           \
        $$PWD/imagesuspect.h      \
        $$PWD/grayscale_suspect.h \

    SOURCES +=                      \
        $$PWD/suspect.cpp           \
        $$PWD/imagesuspect.cpp      \
        $$PWD/grayscale_suspect.cpp \

}
