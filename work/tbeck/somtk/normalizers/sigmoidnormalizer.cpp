#include "sigmoidnormalizer.h"

namespace somtk {

SigmoidNormalizer::SigmoidNormalizer() : Normalizer(){}



void SigmoidNormalizer::initialize( QMap<QString, QVariant> nomalizerParameters )
{
    bool ok = true;

    SOMError::requireCondition(
                nomalizerParameters.contains( "epsilon" ),
                "Normalizer parameters do not contain an epsilon"
                );
    epsilon = nomalizerParameters["epsilon"].toDouble( &ok );
    SOMError::requireCondition( ok, "Couldn't convert epsilon value" );
    SOMError::requireCondition( epsilon > 0, "Epsilon value must be greater than 0.0" );
    SOMError::requireCondition( epsilon < 1.0, "Epsilon value must be less than 1.0" );

    SOMError::requireCondition(
                nomalizerParameters.contains( "sigmaStep" ),
                "Normalizer parameters do not include a sigma step"
                );
    sigmaStep = nomalizerParameters["sigmaStep"].toDouble( &ok );
    SOMError::requireCondition( ok, "Couldn't convert sigma step" );
    SOMError::requireCondition( sigmaStep > 0.0, "Sigma step must be greater than 0" );
}



void SigmoidNormalizer::calculate( QVector<FeaturePtr> features )
{
    int featureSize  = features.first()->size();
    int featureCount = features.size();

    normMean  = QVector<double>( featureSize, 0.0 );
    normStdv  = QVector<double>( featureSize, 0.0 );
    normAlpha = QVector<double>( featureSize, 0.0 );

    foreach( FeaturePtr feature, features )
    {
        QVector<double>& f = *feature.data();
        for( int i = 0; i < featureSize; i++ )
        {
            normMean[i] += f[i];
        }
    }

    for( int i = 0; i < featureSize; i++ )
    {
        normMean[i] /= featureCount;
    }

    foreach( FeaturePtr feature, features )
    {
        QVector<double>& f = *feature.data();
        for( int i = 0; i < featureSize; i++ )
        {
            normStdv[i] += pow2( f[i] - normMean[i] );
        }
    }

    for( int i = 0; i < featureSize; i++ )
    {
        normStdv[i]  = sqrt( normStdv[i] / featureCount );
        normAlpha[i] = log( 1 / epsilon - 1 ) / ( sigmaStep * normStdv[i] );
    }
}



void SigmoidNormalizer::normalize( FeaturePtr feature )
{
    QVector<double>& v = *feature.data();
    for( int i = 0; i < v.size(); i++ )
        v[i] = 1 / ( 1 + exp( -normAlpha[i] * ( v[i] - normMean[i] ) ) );
}



void SigmoidNormalizer::set( FeaturePtr feature )
{
    /// @todo  Use parameters to caluclate directly instead of using two step procedure
    QVector<double>& f = *feature.data();
    for( int i = 0; i < f.size(); i++ )
        f[i] = randomizer.randg( normMean[i], normStdv[i] );
    normalize( feature );
}

} // namespace
