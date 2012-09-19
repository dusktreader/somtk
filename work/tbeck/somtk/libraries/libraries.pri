!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    include( $$SOMTK_ROOT/suspects/suspects.pri )
    include( $$SOMTK_ROOT/features/features.pri )

    HEADERS +=                   \
        $$PWD/library.h          \

    SOURCES +=                   \
        $$PWD/library.cpp        \
}
