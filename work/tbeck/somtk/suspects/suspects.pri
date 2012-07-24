!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    include( $$SOMTK_ROOT/features/features.pri )

    HEADERS +=               \
        $$PWD/suspect.h      \
        $$PWD/colorsuspect.h \

    SOURCES +=                 \
        $$PWD/suspect.cpp      \
        $$PWD/colorsuspect.cpp \

}
