#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>

#include "grids/quadgrid.hpp"
#include "histograms/histogram.h"

#include <iostream>

using namespace std;
using namespace somtk;

class HistogramTest : public QObject
{
    Q_OBJECT

private:

public:
    HistogramTest();



private Q_SLOTS:

    void visualTest();
};

HistogramTest::HistogramTest(){}

void HistogramTest::visualTest()
{

    // Create the grid that the SOM will use
    QVector<int> gridSize;
    gridSize << 5 << 5;
    HistogramGrid grid( new QuadGrid<double>( gridSize ) );
    grid->setTo( 0.0 );

    Histogram histogram( grid );

    histogram.visualize().save( "initalGrid.png" );

    for( int i = 0; i < grid->capacity(); i++ )
        for( int j = 0; j < i; j++ )
            histogram.increment( i );

    histogram.normalize();

    histogram.visualize().save( "finalGrid.png" );

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(HistogramTest)

#include "tst_histogramtest.moc"
