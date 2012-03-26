#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>
#include <QTextStream>

#include "hexgrid.hpp"
#include "som.h"
#include "tools/randmaster.h"

using namespace hsom;

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
    HexGrid<int> grid( QSize( 6, 6 ) );
    QSet<int> testNeighborhood;
    QSet<int> trueNeighborhood;
    QTextStream cout( stdout );


    testNeighborhood = grid.neighborhood( 3, QPoint( 0, 0 ) ).toList().toSet();
    trueNeighborhood << 0 << 1 << 2 << 4 << 5 << 6 << 7 << 10 << 11 << 12 << 13 << 17 << 24 << 25 << 29 << 30 << 31 << 34 << 35;

    cout << "testNeighborhood: ";
    foreach( int index, testNeighborhood )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighborhood: ";
    foreach( int index, trueNeighborhood )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborhood == trueNeighborhood, "Failure");

    trueNeighborhood.clear();
    testNeighborhood = grid.neighborhood( 2, QPoint( 5, 2 ) ).toList().toSet();
    trueNeighborhood << 10 << 11 << 12 << 16 << 17 << 22 << 23;

    cout << "testNeighborhood: ";
    foreach( int index, testNeighborhood )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighborhood: ";
    foreach( int index, trueNeighborhood )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborhood == trueNeighborhood, "Failure");

    trueNeighborhood.clear();
    testNeighborhood = grid.neighborhood( 4, 20 ).toList().toSet();
    trueNeighborhood << 1 << 2 << 3 << 4 << 6 << 7 << 8 << 9 << 10 << 12 << 13 << 14 << 15 << 16 << 17 << 18 << 19 << 20 << 21 << 22 << 23 << 24 << 25 << 26 << 27 << 28 << 29 << 30 << 31 << 32 << 33 << 34;

    cout << "testNeighborhood: ";
    foreach( int index, testNeighborhood )
        cout << index << " ";
    cout << endl;

    cout << "trueNeighborhood: ";
    foreach( int index, trueNeighborhood )
        cout << index << " ";
    cout << endl;

    QVERIFY2(testNeighborhood == trueNeighborhood, "Failure");
}

QTEST_APPLESS_MAIN(HexGridTest)

#include "tst_hexgridtest.moc"

