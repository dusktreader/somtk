!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    HEADERS +=                \
        $$PWD/classifier.h    \
        $$PWD/annclassifier.h \

    SOURCES +=                  \
        $$PWD/classifier.cpp    \
        $$PWD/annclassifier.cpp \

}
