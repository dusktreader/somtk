#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>
#include <QTextStream>

#include "grids/fasthexgrid.hpp"
#include "som.h"
#include "tools/randmaster.h"

using namespace hsom;

class FastHexGridTest : public QObject
{
    Q_OBJECT

public:
    FastHexGridTest();

private Q_SLOTS:
    void distanceTest();
    void neighborhoodTest();
};

FastHexGridTest::FastHexGridTest()
{
}

void FastHexGridTest::distanceTest()
{
    FastHexGrid<int> grid( 4 );

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
    idx1 = 15;
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
    idx1 = 12;
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
    idx0 = 2;
    idx1 = 13;
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
    idx0 = 15;
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

    targetDistance = 3;
    idx0 = 15;
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
    idx0 = 15;
    idx1 = 12;
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
    idx0 = 10;
    idx1 = 4;
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


void FastHexGridTest::neighborhoodTest()
{
    FastHexGrid<int> grid( 4 );
    QVector< QPair<int, int> > neighborhood;
    QSet<int> testNeighborhoodIndices;
    QSet<int> trueNeighborhoodIndices;
    QTextStream cout( stdout );

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
}

QTEST_APPLESS_MAIN(FastHexGridTest)

#include "tst_fasthexgridtest.moc"

