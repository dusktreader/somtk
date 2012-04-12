#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>
#include <QTextStream>

#include "grids/hexgrid.hpp"
#include "som.h"
#include "tools/randmaster.h"

using namespace somtk;

class HexGridTest : public QObject
{
    Q_OBJECT

public:
    HexGridTest();

private Q_SLOTS:
    void neighborhoodTest();
};

HexGridTest::HexGridTest()
{
}

void HexGridTest::neighborhoodTest()
{
    HexGrid<int> grid( 6, 6 );
    QVector< QPair<int, int> > neighborhood;
    QSet<int> testNeighborhoodIndices;
    QSet<int> trueNeighborhoodIndices;
    QTextStream cout( stdout );

    testNeighborhoodIndices.clear();
    neighborhood = grid.neighborhood( 3, 0 );
    for( int i=0; i<neighborhood.size(); i++ )
        testNeighborhoodIndices.insert( neighborhood[i].first );
    trueNeighborhoodIndices << 0 << 1 << 2 << 4 << 5 << 6 << 7 << 10 << 11 << 12 << 13 << 17 << 24 << 25 << 29 << 30 << 31 << 34 << 35;

    cout << "testNeighborhood: ";
    foreach( int index, testNeighborhoodIndices )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighborhood: ";
    foreach( int index, trueNeighborhoodIndices )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborhoodIndices == trueNeighborhoodIndices, "Failure");

    testNeighborhoodIndices.clear();
    neighborhood = grid.neighborhood( 2, 17 );
    for( int i=0; i<neighborhood.size(); i++ )
        testNeighborhoodIndices.insert( neighborhood[i].first );
    trueNeighborhoodIndices << 10 << 11 << 12 << 16 << 17 << 22 << 23;

    cout << "testNeighborhood: ";
    foreach( int index, testNeighborhoodIndices )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighborhood: ";
    foreach( int index, trueNeighborhoodIndices )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborhoodIndices == trueNeighborhoodIndices, "Failure");

    testNeighborhoodIndices.clear();
    neighborhood = grid.neighborhood( 4, 20 );
    for( int i=0; i<neighborhood.size(); i++ )
        testNeighborhoodIndices.insert( neighborhood[i].first );
    trueNeighborhoodIndices << 1 << 2 << 3 << 4 << 6 << 7 << 8 << 9 << 10 << 12 << 13 << 14 << 15 << 16 << 17 << 18 << 19 << 20 << 21 << 22 << 23 << 24 << 25 << 26 << 27 << 28 << 29 << 30 << 31 << 32 << 33 << 34;

    cout << "testNeighborhood: ";
    foreach( int index, testNeighborhoodIndices )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighborhood: ";
    foreach( int index, trueNeighborhoodIndices )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborhoodIndices == trueNeighborhoodIndices, "Failure");
}

QTEST_APPLESS_MAIN(HexGridTest)

#include "tst_hexgridtest.moc"

