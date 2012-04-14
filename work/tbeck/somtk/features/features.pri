!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    HEADERS +=          \
        $$PWD/feature.h \

    SOURCES +=            \
        $$PWD/feature.cpp \

}
