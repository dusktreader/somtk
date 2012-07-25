#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QTextStream>

#include "grids/quadgrid.hpp"

using namespace somtk;

class QuadGridTest : public QObject
{
    Q_OBJECT

public:
    QuadGridTest();

private Q_SLOTS:
    void distanceTest();
    void neighborTest();
    void neighborhoodTest();
};

QuadGridTest::QuadGridTest()
{
}

void QuadGridTest::distanceTest()
{
    // 00 01 02 03 04 05
    // 06 07 08 09 10 11
    // 12 13 14 15 16 17
    // 18 19 20 21 22 23
    // 24 25 26 27 28 29
    // 30 31 32 33 34 35
    QuadGrid<int> grid( 6, 6 );

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

    targetDistance = 4;
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

    targetDistance = 5;
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

    targetDistance = 6;
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

void QuadGridTest::neighborTest()
{
    // 00 01 02 03 04 05
    // 06 07 08 09 10 11
    // 12 13 14 15 16 17
    // 18 19 20 21 22 23
    // 24 25 26 27 28 29
    // 30 31 32 33 34 35
    QuadGrid<int> grid( 6, 6 );
    QVector<int> neighbors;
    QSet<int> testNeighborIndices;
    QSet<int> trueNeighborIndices;
    QTextStream cout( stdout );

    testNeighborIndices.clear();
    trueNeighborIndices.clear();
    neighbors = grid.neighbors( 7 );
    for( int i=0; i<neighbors.size(); i++ )
        testNeighborIndices.insert( neighbors[i] );
    trueNeighborIndices << 1 << 6 << 8 << 13;

    cout << "testNeighbors: ";
    foreach( int index, testNeighborIndices )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighbors: ";
    foreach( int index, trueNeighborIndices )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborIndices == trueNeighborIndices, "Failure");



    testNeighborIndices.clear();
    trueNeighborIndices.clear();
    neighbors = grid.neighbors( 0 );
    for( int i=0; i<neighbors.size(); i++ )
        testNeighborIndices.insert( neighbors[i] );
    trueNeighborIndices << 1 << 6 << 5 << 30;

    cout << "testNeighbors: ";
    foreach( int index, testNeighborIndices )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighbors: ";
    foreach( int index, trueNeighborIndices )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborIndices == trueNeighborIndices, "Failure");



    testNeighborIndices.clear();
    trueNeighborIndices.clear();
    neighbors = grid.neighbors( 35 );
    for( int i=0; i<neighbors.size(); i++ )
        testNeighborIndices.insert( neighbors[i] );
    trueNeighborIndices << 29 << 34 << 5 << 30;

    cout << "testNeighbors: ";
    foreach( int index, testNeighborIndices )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighbors: ";
    foreach( int index, trueNeighborIndices )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborIndices == trueNeighborIndices, "Failure");
}

void QuadGridTest::neighborhoodTest()
{
    // 00 01 02 03 04 05
    // 06 07 08 09 10 11
    // 12 13 14 15 16 17
    // 18 19 20 21 22 23
    // 24 25 26 27 28 29
    // 30 31 32 33 34 35
    QuadGrid<int> grid( 6, 6 );
    QVector< QPair<int, int> > neighborhood;
    QSet<int> testNeighborhoodIndices;
    QSet<int> trueNeighborhoodIndices;
    QTextStream cout( stdout );

    testNeighborhoodIndices.clear();
    neighborhood = grid.neighborhood( 2, 0 );
    for( int i=0; i<neighborhood.size(); i++ )
        testNeighborhoodIndices.insert( neighborhood[i].first );
    trueNeighborhoodIndices
                        << 24
                  << 35 << 30 << 31
            <<  4 <<  5 <<  0 <<  1 <<  2
                  << 11 << 06 << 07
                        << 12;

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

QTEST_APPLESS_MAIN(QuadGridTest)

#include "tst_quadgridtest.moc"

