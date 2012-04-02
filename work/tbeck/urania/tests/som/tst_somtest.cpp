#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>

#include "normalizers/nullnormalizer.h"
#include "som.h"
#include "tools/randmaster.h"

using namespace hsom;

class SomTest : public QObject
{
    Q_OBJECT

private:

    QImage visualizeColorGrid( HexGrid<Feature> grid );

public:
    SomTest();

private Q_SLOTS:
    void visualTest();
};

SomTest::SomTest()
{
}

QImage SomTest::visualizeColorGrid( HexGrid<Feature> grid )
{
    double radius = 10.0;
    double scale  = 20.0;
    QImage viz = QImage( QSize(scale * grid.size().width()  + radius,
                               scale * grid.size().height() + radius ),
                         QImage::Format_RGB888 );

    viz.fill( Qt::black );
    QPainter vizPainter;
    vizPainter.begin( &viz );

    for( int i=0; i<grid.size().height(); i++ )
    {
        for( int j=0; j<grid.size().width(); j++ )
        {
            QPoint coords = QPoint(j, i);
            QPointF realCoords = grid.realCoords( coords, scale );
            realCoords.setX( realCoords.x() + radius );
            realCoords.setY( realCoords.y() + radius );
            Feature feature = grid[ coords ];
            int r = (int)( feature[0] * 255 );
            int g = (int)( feature[1] * 255 );
            int b = (int)( feature[2] * 255 );
            QColor color( r, g, b );

            /*
            printVar( i, "i" );
            printVar( j, "j" );
            printVar( feature[0], "feature[0]" );
            printVar( feature[1], "feature[1]" );
            printVar( feature[2], "feature[2]" );
            printVar( r, "r" );
            printVar( g, "g" );
            printVar( b, "b" );
            */

            vizPainter.setPen( QPen( QColor( 255, 255, 255 ) ) );
            vizPainter.setBrush( QBrush( color ) );
            vizPainter.drawEllipse( realCoords, radius, radius );
        }
    }
    vizPainter.end();
    return viz;
}

void SomTest::visualTest()
{
    NormalizerPtr normalizer( new NullNormalizer() );

    QSize size( 24, 24 );

    SOMPtr som( new SOM( size ) );
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
    QVector<Feature> untrainedFeatures = som->dumpFeatures();
    HexGrid<Feature> initialGrid( size, untrainedFeatures );
    QImage initialGridImage = visualizeColorGrid( initialGrid );
    initialGridImage.save( "initialGrid.png" );

    som->train( inputFeatures, normalizer, somParameters, true );

    QVector<Feature>trainedFeatures = som->dumpFeatures();
    HexGrid<Feature> finalGrid( size, trainedFeatures );
    QImage finalGridImage = visualizeColorGrid( finalGrid );
    finalGridImage.save( "finalGrid.png" );

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SomTest)

#include "tst_somtest.moc"
