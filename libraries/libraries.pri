!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    include( $$SOMTK_ROOT/suspects/suspects.pri )
    include( $$SOMTK_ROOT/features/features.pri )

    HEADERS +=                     \
        $$PWD/suspect_library.h    \
        $$PWD/image_library.h      \
        $$PWD/sobel_hu_library.h   \

    SOURCES +=                     \
        $$PWD/suspect_library.cpp  \
        $$PWD/image_library.cpp    \
        $$PWD/sobel_hu_library.cpp \
}
