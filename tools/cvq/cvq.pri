!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    HEADERS +=             \
        $$PWD/cvq.h \

    SOURCES +=               \
        $$PWD/cvq.cpp \
}
