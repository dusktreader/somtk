!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    message( "Including $$PWD" )

    HEADERS +=                \
        $$PWD/grid.hpp        \
        $$PWD/quadgrid.hpp    \
        $$PWD/hexgrid.hpp     \
        $$PWD/fasthexgrid.hpp \
        $$PWD/wraphexgrid.hpp \
}
