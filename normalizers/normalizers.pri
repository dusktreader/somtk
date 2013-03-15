!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    include( $$SOMTK_ROOT/tools/randmaster/randmaster.pri )
    include( $$SOMTK_ROOT/features/features.pri )

    HEADERS +=                      \
        $$PWD/normalizer.h          \
        $$PWD/nullnormalizer.h      \
        $$PWD/minmaxnormalizer.h    \
        $$PWD/sigmoidnormalizer.h   \

    SOURCES +=                      \
        $$PWD/normalizer.cpp        \
        $$PWD/nullnormalizer.cpp    \
        $$PWD/minmaxnormalizer.cpp  \
        $$PWD/sigmoidnormalizer.cpp \

}
