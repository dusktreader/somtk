!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    include( $$SOMTK_ROOT/grids/grids.pri )
    include( $$SOMTK_ROOT/normalizers/normalizers.pri )
    include( $$SOMTK_ROOT/features/features.pri )

    HEADERS +=      \
        $$PWD/som.h \

    SOURCES +=        \
        $$PWD/som.cpp \

}
