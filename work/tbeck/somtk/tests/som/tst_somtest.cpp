#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>

#include "soms/som.h"
#include "normalizers/nullnormalizer.h"
#include "normalizers/minmaxnormalizer.h"
#include "normalizers/sigmoidnormalizer.h"
#include "grids/quadgrid.hpp"
#include "grids/hexgrid.hpp"
#include "grids/fasthexgrid.hpp"
#include "grids/wraphexgrid.hpp"
#include "tools/randmaster.h"

#include <iostream>
using namespace std;
using namespace somtk;

class SomTest : public QObject
{
    Q_OBJECT

private:

public:
    SomTest();

private Q_SLOTS:
    void visualTest();
};

SomTest::SomTest(){}

QColor render( FeaturePtr feature )
{
    Feature& f = *feature.data();
    int r = (int)( f[0] * 255 );
    int g = (int)( f[1] * 255 );
    int b = (int)( f[2] * 255 );
    return QColor( r, g, b );
}

void SomTest::visualTest()
{

    QVector<int> size;
    size << 24 << 24;
    FeatureGrid grid( new QuadGrid<FeaturePtr>( size ) );
    SOMPtr som( new SOM( grid ) );
    RandMaster rnd;


    QVector<FeaturePtr> inputFeatures;
    for( int i=0; i<1000; i++ )
    {
        FeaturePtr feature( new Feature( 3 ) );
        Feature& f = *feature.data();
        double r = rnd.randd(-100.0, 100.0);
        double g = rnd.randd(10000.0,20000.0);
        double b = rnd.randd(0.001, 0.002);
        f[0] = r;
        f[1] = g;
        f[2] = b;
        inputFeatures.append( feature );
    }

    QMap<QString, QVariant> normalizerParameters;
    //normalizerParameters["HiLimit"] = 1.0;
    //normalizerParameters["LoLimit"] = 0.0;
    //NormalizerPtr normalizer( new MinMaxNormalizer() );
    normalizerParameters["epsilon"] = 0.0625;
    normalizerParameters["sigmaStep"] = 2;
    NormalizerPtr normalizer( new SigmoidNormalizer() );

    normalizer->calculate( inputFeatures, normalizerParameters );
    normalizer->normalize( inputFeatures );

    QMap<QString, QVariant> somParameters;
    somParameters["maxEpochs"] = 40;
    somParameters["initialAlpha"] = 0.8;
    somParameters["initialRadiusRatio"] = 0.4999;

    som->initializeTraining( somParameters, normalizer, inputFeatures.front()->size() );
    som->grid()->visualize( 10, &render ).save( "initialGrid.png" );

    som->train( inputFeatures, normalizer, somParameters, true );

    som->grid()->visualize( 10, &render ).save( "finalGrid.png" );

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SomTest)

#include "tst_somtest.moc"
