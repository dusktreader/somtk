#include <QtCore/QString>
#include <QtTest/QtTest>

#include <iostream>

#include "features/feature.h"
#include "normalizers/normalizer.h"
#include "normalizers/sigmoidnormalizer.h"

using namespace std;
using namespace somtk;

class SigmoidNormalizerTest : public QObject
{
    Q_OBJECT

public:
    SigmoidNormalizerTest();

private Q_SLOTS:
    void testCase1();
};

SigmoidNormalizerTest::SigmoidNormalizerTest(){}

void SigmoidNormalizerTest::testCase1()
{
    int featureSize = 10;
    int featureCount = 1000;

    QMap<QString, QVariant> normalizerParameters;
    normalizerParameters["epsilon"] = 1.0e-10;
    normalizerParameters["sigmaStep"] = 10;
    SigmoidNormalizer normalizer;

    RandMaster randomizer;
    QVector<double> possibleLoRanges( featureSize );
    QVector<double> possibleHiRanges( featureSize );
    QVector<double> originalLoRanges( featureSize );
    QVector<double> originalHiRanges( featureSize );
    QVector<double> normalizedLoRanges( featureSize );
    QVector<double> normalizedHiRanges( featureSize );

    QVector<double> originalMeans( featureSize, 0.0 );
    QVector<double> originalStdvs( featureSize, 0.0 );
    QVector<double> normalizedMeans( featureSize, 0.0 );
    QVector<double> normalizedStdvs( featureSize, 0.0 );

    for( int j=0; j<featureSize; j++ )
    {
        possibleLoRanges[j] = randomizer.randd( -1000.0, 999.0 );
        do
        {
            possibleHiRanges[j] = randomizer.randd( -999.0, 1000.0 );
        } while( possibleHiRanges[j] <= possibleLoRanges[j] );
        originalLoRanges[j] = DBL_MAX;
        originalHiRanges[j] = DBL_MIN;
        normalizedLoRanges[j] = DBL_MAX;
        normalizedHiRanges[j] = DBL_MIN;
    }

    QVector<FeaturePtr> normalizedFeatures;
    QVector<FeaturePtr> originalFeatures;
    for( int i=0; i<featureCount; i++ )
    {
        normalizedFeatures.push_back( FeaturePtr( new Feature( featureSize ) ) );
        Feature& feature = *normalizedFeatures.back();
        for( int j=0; j<featureSize; j++ )
            feature[j] = randomizer.randd( possibleLoRanges[j], possibleHiRanges[j] );
        originalFeatures.push_back( FeaturePtr( new Feature( *normalizedFeatures.back() ) ) );
    }

    normalizer.calculate( normalizedFeatures, normalizerParameters );
    normalizer.normalizeAll( normalizedFeatures );


    for( int i=0; i<featureCount; i++ )
    {
        Feature& originalFeature = *(originalFeatures[i]);
        Feature& normalizedFeature = *(normalizedFeatures[i]);
        cout << i << ":" << endl;
        for( int j=0; j<featureSize; j++ )
        {
            originalLoRanges[j] = min( originalLoRanges[j], originalFeature[j] );
            originalHiRanges[j] = max( originalHiRanges[j], originalFeature[j] );
            normalizedLoRanges[j] = min( normalizedLoRanges[j], normalizedFeature[j] );
            normalizedHiRanges[j] = max( normalizedHiRanges[j], normalizedFeature[j] );

            originalMeans[j] += originalFeature[j];
            normalizedMeans[j] += normalizedFeature[j];

            cout << "    " << originalFeature[j] << " -> " << normalizedFeature[j] << endl;
            QVERIFY2( normalizedFeature[j] >= 0 && normalizedFeature[j] <= 1,
                      QString( "Value out of range in %1th feature "
                               "at index %2: %3 normalized to %4" )
                      .arg( i ).arg( j )
                      .arg( originalFeature[j] ).arg( normalizedFeature[j] )
                      .toAscii() );
        }
    }

    for( int j=0; j<featureSize; j++ )
    {
        originalMeans[j] /= featureCount;
        normalizedMeans[j] /= featureCount;
    }

    for( int i=0; i<featureCount; i++ )
    {
        for( int j=0; j<featureSize; j++ )
        {
            Feature& originalFeature = *(originalFeatures[i]);
            Feature& normalizedFeature = *(normalizedFeatures[i]);

            originalStdvs[j] += pow2( originalFeature[j] - originalMeans[j] );
            normalizedStdvs[j] += pow2( normalizedFeature[j] - normalizedMeans[j] );
        }
    }

    for( int j=0; j<featureSize; j++ )
    {
        originalStdvs[j] = sqrt( originalStdvs[j] / featureCount );
        normalizedStdvs[j] = sqrt( normalizedStdvs[j] / featureCount );
    }

    for( int j=0; j<featureSize; j++ )
    {
        cout << "Index " << j << endl;
        cout << "    possible input value range = [ " << possibleLoRanges[j] << ", "
             << possibleHiRanges[j] << "]" << endl;
        cout << "    actual input value range = [ " << originalLoRanges[j] << ", "
             << originalHiRanges[j] << "]" << endl;
        cout << "    normalized value range = [ " << normalizedLoRanges[j] << ", "
             << normalizedHiRanges[j] << "]" << endl;
        cout << "    original mean=" << originalMeans[j] << endl;
        cout << "    original stdv=" << originalStdvs[j] << endl;
        cout << "    normalized mean=" << normalizedMeans[j] << endl;
        cout << "    normalized stdv=" << normalizedStdvs[j] << endl;
    }
}

QTEST_APPLESS_MAIN(SigmoidNormalizerTest)

#include "tst_sigmoidnormalizertest.moc"
