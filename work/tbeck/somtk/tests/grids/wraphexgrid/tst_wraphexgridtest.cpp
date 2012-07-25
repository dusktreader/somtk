#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QTextStream>

#include "grids/wraphexgrid.hpp"

using namespace somtk;

class WrapHexGridTest : public QObject
{
    Q_OBJECT

public:
    WrapHexGridTest();

private Q_SLOTS:
    void distanceTest();
    void neighborTest();
    void neighborhoodTest();
};

WrapHexGridTest::WrapHexGridTest()
{
}

void WrapHexGridTest::distanceTest()
{
    WrapHexGrid<int> grid( 6 );

    int targetDistance, actualDistance;
    int idx0, idx1;

    targetDistance = 3;
    idx0 = 0;
    idx1 = 3;
    actualDistance = grid.distance( idx0, idx1 );
    QVERIFY2( targetDistance == actualDistance,
              QString( "Distance incorrect: dist( %1, %2 ) should be %3, was %4")
              .arg( idx0 )
              .arg( idx1 )
              .arg( targetDistance )
              .arg( actualDistance )
              .toAscii()
              );

    targetDistance = 3;
    idx0 = 0;
    idx1 = 18;
    actualDistance = grid.distance( idx0, idx1 );
    QVERIFY2( targetDistance == actualDistance,
              QString( "Distance incorrect: dist( %1, %2 ) should be %3, was %4")
              .arg( idx0 )
              .arg( idx1 )
              .arg( targetDistance )
              .arg( actualDistance )
              .toAscii()
              );

    targetDistance = 4;
    idx0 = 7;
    idx1 = 21;
    actualDistance = grid.distance( idx0, idx1 );
    QVERIFY2( targetDistance == actualDistance,
              QString( "Distance incorrect: dist( %1, %2 ) should be %3, was %4")
              .arg( idx0 )
              .arg( idx1 )
              .arg( targetDistance )
              .arg( actualDistance )
              .toAscii()
              );

    targetDistance = 3;
    idx0 = 9;
    idx1 = 26;
    actualDistance = grid.distance( idx0, idx1 );
    QVERIFY2( targetDistance == actualDistance,
              QString( "Distance incorrect: dist( %1, %2 ) should be %3, was %4")
              .arg( idx0 )
              .arg( idx1 )
              .arg( targetDistance )
              .arg( actualDistance )
              .toAscii()
              );

    targetDistance = 3;
    idx0 = 10;
    idx1 = 31;
    actualDistance = grid.distance( idx0, idx1 );
    QVERIFY2( targetDistance == actualDistance,
              QString( "Distance incorrect: dist( %1, %2 ) should be %3, was %4")
              .arg( idx0 )
              .arg( idx1 )
              .arg( targetDistance )
              .arg( actualDistance )
              .toAscii()
              );

    targetDistance = 3;
    idx0 = 25;
    idx1 = 10;
    actualDistance = grid.distance( idx0, idx1 );
    QVERIFY2( targetDistance == actualDistance,
              QString( "Distance incorrect: dist( %1, %2 ) should be %3, was %4")
              .arg( idx0 )
              .arg( idx1 )
              .arg( targetDistance )
              .arg( actualDistance )
              .toAscii()
              );

    targetDistance = 2;
    idx0 = 35;
    idx1 = 0;
    actualDistance = grid.distance( idx0, idx1 );
    QVERIFY2( targetDistance == actualDistance,
              QString( "Distance incorrect: dist( %1, %2 ) should be %3, was %4")
              .arg( idx0 )
              .arg( idx1 )
              .arg( targetDistance )
              .arg( actualDistance )
              .toAscii()
              );

    targetDistance = 4;
    idx0 = 25;
    idx1 = 3;
    actualDistance = grid.distance( idx0, idx1 );
    QVERIFY2( targetDistance == actualDistance,
              QString( "Distance incorrect: dist( %1, %2 ) should be %3, was %4")
              .arg( idx0 )
              .arg( idx1 )
              .arg( targetDistance )
              .arg( actualDistance )
              .toAscii()
              );
}

void WrapHexGridTest::neighborTest()
{
    WrapHexGrid<int> grid( 6 ) ;
    QVector<int> neighbors;

    QSet<int> testNeighborIndices;
    QSet<int> trueNeighborIndices;

    QTextStream cout( stdout );

    testNeighborIndices.clear();
    trueNeighborIndices.clear();
    neighbors = grid.neighbors( 3 );
    foreach( int index, neighbors )
        testNeighborIndices.insert( index );
    trueNeighborIndices << 2 << 4 << 8 << 9 << 33 << 34;

    cout << "testNeighbors: ";
    foreach( int index, testNeighborIndices )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighbors: ";
    foreach( int index, trueNeighborIndices )
        cout << index << " ";
    cout << endl;
}

void WrapHexGridTest::neighborhoodTest()
{
    WrapHexGrid<int> grid( 4 );
    QVector< QPair<int, int> > neighborhood;
    QSet<int> testNeighborhoodIndices;
    QSet<int> trueNeighborhoodIndices;
    QTextStream cout( stdout );
/*
    testNeighborhoodIndices.clear();
    neighborhood = grid.neighborhood( 3, 0 );
    for( int i=0; i<neighborhood.size(); i++ )
        testNeighborhoodIndices.insert( neighborhood[i].first );
    trueNeighborhoodIndices << 0 << 1 << 2 << 4 << 5 << 6 << 8 << 9 << 10;

    cout << "testNeighborhood: ";
    foreach( int index, testNeighborhoodIndices )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighborhood: ";
    foreach( int index, trueNeighborhoodIndices )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborhoodIndices == trueNeighborhoodIndices, "Failure");
 */
}

QTEST_APPLESS_MAIN(WrapHexGridTest)

#include "tst_wraphexgridtest.moc"

