#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>

#include "som.h"
#include "normalizers/nullnormalizer.h"
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

SomTest::SomTest()
{
    cout << "creating test" << endl;
}

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
    cout << "visualTest" << endl;
    NormalizerPtr normalizer( new NullNormalizer() );

    QVector<int> size;
    size << 24 << 24;
    QuadGrid<FeaturePtr> grid( size );
    SOMPtr som( new SOM( grid ) );
    RandMaster rnd;


    QVector<FeaturePtr> inputFeatures;
    for( int i=0; i<1000; i++ )
    {
        FeaturePtr feature( new Feature( 3 ) );
        Feature& f = *feature.data();
        double r = rnd.randd();
        double g = rnd.randd();
        double b = rnd.randd();
        f[0] = r;
        f[1] = g;
        f[2] = b;
        inputFeatures.append( feature );
    }

    QMap<QString, QVariant> somParameters;
    somParameters["maxEpochs"] = 40;
    somParameters["initialAlpha"] = 0.8;
    somParameters["initialRadiusRatio"] = 0.4999;

    som->initializeTraining( somParameters, normalizer, inputFeatures.front()->size() );
    grid.visualize( 10, &render ).save( "initialGrid.png" );

    som->train( inputFeatures, normalizer, somParameters, true );

    grid.visualize( 10, &render ).save( "finalGrid.png" );

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SomTest)

#include "tst_somtest.moc"
