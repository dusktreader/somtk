#include <QString>

#include "grids/fasthexgrid.hpp"
#include "suspects/colorsuspect.h"
#include "soms/som.h"
//#include "classifiers/annclassifier.h"
#include "library.h"
//#include "hsom.h"

using namespace somtk;

int main(int argc, char *argv[])
{
    QVector<int> gridSize;
    gridSize << 25;

    HistogramGrid gridTemplate( new FastHexGrid<double>( gridSize ) );
    Library library;
    library.load( "/data/code/data/somtk/hsom/apple_strawberry_library/library.xml", gridTemplate );
    return 0;
}
