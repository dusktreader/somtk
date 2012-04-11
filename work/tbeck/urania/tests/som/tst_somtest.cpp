#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>
#include <iostream>

#include "som.h"
#include "normalizers/nullnormalizer.h"
#include "grids/quadgrid.hpp"
#include "grids/hexgrid.hpp"
#include "tools/randmaster.h"

using namespace hsom;

class SomTest : public QObject
{
    Q_OBJECT

private:

public:
    SomTest();

private Q_SLOTS:
    void visualTest();
};

SomTest::SomTest()
{
}

QColor render( Feature feature )
{
    int r = (int)( feature[0] * 255 );
    int g = (int)( feature[1] * 255 );
    int b = (int)( feature[2] * 255 );
    return QColor( r, g, b );
}

void SomTest::visualTest()
{
    NormalizerPtr normalizer( new NullNormalizer() );

    QVector<int> size;
    size << 24 << 24;
    HexGrid<Feature> grid( size );
    SOMPtr som( new SOM( grid ) );
    RandMaster rnd;


    QVector<Feature> inputFeatures;
    for( int i=0; i<1000; i++ )
    {
        Feature f( 3 );
        double r = rnd.randd();
        double g = rnd.randd();
        double b = rnd.randd();
        f[0] = r;
        f[1] = g;
        f[2] = b;
        inputFeatures.append( f );
    }

    QMap<QString, QVariant> somParameters;
    somParameters["maxEpochs"] = 40;
    somParameters["initialAlpha"] = 0.8;
    somParameters["initialRadiusRatio"] = 0.4999;

    som->initializeTraining( somParameters, normalizer, inputFeatures.front().size() );
    grid.visualize( 10, &render ).save( "initialGrid.png" );

    som->train( inputFeatures, normalizer, somParameters, true );

    grid.visualize( 10, &render ).save( "finalGrid.png" );

    for( int i=0; i<inputFeatures.size(); i++ )
    {
        Feature feature = inputFeatures[i];
        for(int j=0; j<feature.size(); j++ )
            std::cout << feature[j] << " ";
        std::cout << std::endl;
    }


    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SomTest)

#include "tst_somtest.moc"
