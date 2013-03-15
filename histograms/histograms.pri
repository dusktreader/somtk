!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    include( $$SOMTK_ROOT/grids/grids.pri )

    HEADERS +=            \
        $$PWD/histogram.h \

    SOURCES +=              \
        $$PWD/histogram.cpp \

}
