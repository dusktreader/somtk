#include "sigmoidnormalizer.h"

namespace somtk {

SigmoidNormalizer::SigmoidNormalizer() : Normalizer(){}


void SigmoidNormalizer::calculateNormalizer( QVector<FeaturePtr> features )
{
    bool ok = true;

    ASSERT_MSG( _calculationParameters.contains( "epsilon" ), "Normalizer parameters do not contain an epsilon" );
    epsilon = _calculationParameters["epsilon"].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert epsilon value" );
    ASSERT_MSG( epsilon > 0, "Epsilon value must be greater than 0.0" );
    ASSERT_MSG( epsilon < 1.0, "Epsilon value must be less than 1.0" );

    ASSERT_MSG( _calculationParameters.contains( "sigmaStep" ), "Normalizer parameters do not include a sigma step" );
    sigmaStep = _calculationParameters["sigmaStep"].toInt( &ok );
    ASSERT_MSG( ok, "Couldn't convert sigma step" );
    ASSERT_MSG( sigmaStep > 0, "Sigma step must be greater than 0" );

    int featureSize  = features.first()->size();
    int featureCount = features.size();

    QVector<double> normSum( featureSize, 0.0 );
    normMean  = QVector<double>( featureSize, 0.0 );
    normStdv  = QVector<double>( featureSize, 0.0 );
    normAlpha = QVector<double>( featureSize, 0.0 );

    foreach( FeaturePtr feature, features )
    {
        QVector<double>& f = *feature.data();
        for( int i = 0; i < featureSize; i++ )
        {
            normSum[i] += f[i];
            double t = f[i] - normMean[i];
            normMean[i] += t / featureCount;
            normStdv[i] += t * ( f[i] - normMean[i] );
        }
    }

    for( int i = 0; i < featureSize; i++ )
    {
        normMean[i] = normSum[i] / featureCount;
    }

    foreach( FeaturePtr feature, features )
    {
        QVector<double>& f = *feature.data();
        for( int i = 0; i < featureSize; i++ )
            normStdv[i] += pow2( f[i] - normMean[i] );
    }

    for( int i = 0; i < featureSize; i++ )
    {
        normStdv[i] = sqrt( normStdv[i] / ( features.size() - 1 ) );
        normAlpha[i] = log( 1 / epsilon - 1 ) / ( sigmaStep * normStdv[i] );
    }
}



void SigmoidNormalizer::normalize( FeaturePtr feature )
{
    QVector<double>& v = *feature.data();
    for( int i = 0; i < v.size(); i++ )
        v[i] = 1 / ( 1 + exp( -normAlpha[i] * ( v[i] - normMean[i] ) ) );
}



void SigmoidNormalizer::setFeature( FeaturePtr feature )
{
    QVector<double>& v = *feature.data();
    for( int i = 0; i < v.size(); i++ )
        v[i] = randomizer.randg( normMean[i], normStdv[i] );
}

} // namespace
