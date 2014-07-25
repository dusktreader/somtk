!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    HEADERS +=              \
        $$PWD/randmaster.h  \

    SOURCES +=               \
        $$PWD/randmaster.cpp \
}
