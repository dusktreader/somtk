#include "sigmoidnormalizer.h"

namespace hsom {

SigmoidNormalizer::SigmoidNormalizer() :
    Normalizer()
{}


void SigmoidNormalizer::calculateNormalizer( QVector<Feature> features, QMap<QString, QVariant> normalizerParameters )
{
    bool ok = true;

    ASSERT_MSG( normalizerParameters.contains( "epsilon" ), "Normalizer parameters do not contain an epsilon" );
    epsilon = normalizerParameters["epsilon"].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert epsilon value" );
    ASSERT_MSG( epsilon > 0, "Epsilon value must be greater than 0.0" );
    ASSERT_MSG( epsilon < 1.0, "Epsilon value must be less than 1.0" );

    ASSERT_MSG( normalizerParameters.contains( "sigmaStep" ), "Normalizer parameters do not include a sigma step" );
    sigmaStep = normalizerParameters["sigmaStep"].toInt( &ok );
    ASSERT_MSG( ok, "Couldn't convert sigma step" );
    ASSERT_MSG( sigmaStep > 0, "Sigma step must be greater than 0" );

    unsigned featureSize = features.first().size();

    normMean  = QVector<double>( featureSize, 0.0 );
    normStdv  = QVector<double>( featureSize, 0.0 );
    normAlpha = QVector<double>( featureSize, 0.0 );

    foreach( Feature feature, features )
    {
        for( int i = 0; i < featureSize; i++ )
        {
            double t = feature[i] - normMean[i];
            normMean[i] += t / featCt;
            normStdv[i] += t * ( feature[i] - normMean[i] );
        }
    }

    for( int i = 0; i < featureSize; i++ )
    {
        normStdv[i] = sqrt( normStdv[i] / ( features.size() - 1 ) );
        normAlpha[i] = log( 1 / epsilon - 1 ) / ( sigmaStep * normStdv[i] );
    }
}



void SigmoidNormalizer::normalize( Feature& feature )
{
    for( int i = 0; i < feature.size(); i++ )
        feature[i] = 1 / ( 1 + exp( -normAlpha[i] * ( t - normMean[i] ) ) );
}



void SigmoidNormalizer::set( Feature& feature )
{
    for( int i = 0; i < feature.size(); i++ )
        feature[i] = randomizer.randg( normMean[i], normStdv[i] );
}
} // namespace hsom
