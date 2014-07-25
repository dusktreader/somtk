!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    isEmpty(TOOLS_ROOT) {
        error( "TOOLS_ROOT variable not defined.  Specify the path to tools in your .pro file" )
    }

    HEADERS += \
        $$PWD/local_assert.h \
        $$PWD/local_assert_qt.h \

    SOURCES += \
        $$PWD/local_assert.cpp \

}
