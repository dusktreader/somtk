#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>

#include "soms/som.h"
#include "normalizers/nullnormalizer.h"
#include "normalizers/minmaxnormalizer.h"
#include "normalizers/sigmoidnormalizer.h"
#include "grids/quadgrid.hpp"
#include "grids/fasthexgrid.hpp"
#include "grids/wraphexgrid.hpp"
#include "tools/randmaster/randmaster.h"

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

    // Create the grid that the SOM will use
    QVector<int> gridSize;
    gridSize << 24;
    FeatureGrid grid( new WrapHexGrid<FeaturePtr>( gridSize ) );

    // Create a new RandMaster for generating random values
    RandMaster rnd;

    // Create the input features for the SOM
    QVector<FeaturePtr> features;
    int featureCount = 100;
    for( int i=0; i<featureCount; i++ )
    {
        FeaturePtr feature( new Feature( 3 ) );
        Feature& f = *feature.data();

        // Notice that each input has a different range
        f[0] = rnd.randd(  -100.000,   100.000 );
        f[1] = rnd.randd( 10000.000, 20000.000 );
        f[2] = rnd.randd(     0.001,     0.002 );
        features.append( feature );
    }

    // Create the normalizer for the input data
    QMap<QString, QVariant> normalizerParameters;
    normalizerParameters["epsilon"] = 1.0e-6;
    normalizerParameters["sigmaStep"] = 5;
    NormalizerPtr normalizer( new SigmoidNormalizer() );
    normalizer->initialize( normalizerParameters );

    // Create the SOM
    SOMPtr som( new SOM( grid, normalizer ) );

    QMap<QString, QVariant> somParameters;
    somParameters["maxEpochs"] = 20;
    somParameters["initialAlpha"] = 0.8;
    somParameters["initialRadiusRatio"] = 0.4999;
    som->initializeTraining( somParameters, features );
    som->grid()->visualize( 10, &render ).save( "initialGrid.png" );

    som->train( somParameters, features, true );

    som->grid()->visualize( 10, &render ).save( "finalGrid.png" );

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SomTest)

#include "tst_somtest.moc"
