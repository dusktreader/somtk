#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QVector>
#include <QImageReader>

#include "libraries/library.h"
#include "suspects/sobelhususpect.h"
#include "grids/fasthexgrid.hpp"

using namespace somtk;

class LibraryTest : public QObject
{
    Q_OBJECT

public:
    LibraryTest();

private Q_SLOTS:
    void importTest();
};

LibraryTest::LibraryTest(){}

void LibraryTest::importTest()
{

    qDebug() << QImageReader::supportedImageFormats();
    QVector<int> gridSize;
    gridSize.append( 8 );
    HistogramGrid gridTemplate = HistogramGrid( new FastHexGrid<double>( gridSize ) );
    Library testLibrary;
    testLibrary.load(
                "/data/code/data/apple_strawberry_library/testing_library.xml",
                gridTemplate
                );
}

QTEST_APPLESS_MAIN(LibraryTest)

#include "tst_librarytest.moc"
