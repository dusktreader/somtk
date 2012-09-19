#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QVector>
#include <QImage>

#include "suspects/sobelhususpect.h"
#include "grids/fasthexgrid.hpp"

using namespace somtk;

class SuspectsTest : public QObject
{
    Q_OBJECT

public:
    SuspectsTest();

private Q_SLOTS:
    void extractTest();
};

SuspectsTest::SuspectsTest(){}

void SuspectsTest::extractTest()
{
    QImage image( "/data/code/builds/somtk/tests/suspects/debug/testimage.jpg" );


    QVector<int> gridSize;
    gridSize.append( 8 );
    HistogramGrid gridTemplate = HistogramGrid( new FastHexGrid<double>( gridSize ) );
    SuspectPtr suspect( new SobelHuSuspect( image, gridTemplate ) );
    QVector<FeaturePtr> features = suspect->features();

}

QTEST_APPLESS_MAIN(SuspectsTest)

#include "tst_suspectstest.moc"
