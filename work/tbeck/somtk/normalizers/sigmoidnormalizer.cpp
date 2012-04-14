#include "sigmoidnormalizer.h"

namespace somtk {

SigmoidNormalizer::SigmoidNormalizer() : Normalizer(){}


void SigmoidNormalizer::calculateNormalizer( QVector<DVectorPtr> vectors,
                                             QMap<QString, QVariant> normalizerParameters )
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

    int vectorSize = vectors.first()->size();
    int vectorCount = vectors.size();

    normMean  = QVector<double>( vectorSize, 0.0 );
    normStdv  = QVector<double>( vectorSize, 0.0 );
    normAlpha = QVector<double>( vectorSize, 0.0 );

    foreach( DVectorPtr vector, vectors )
    {
        QVector<double>& f = *vector.data();
        for( int i = 0; i < vectorSize; i++ )
        {
            double t = f[i] - normMean[i];
            normMean[i] += t / vectorCount;
            normStdv[i] += t * ( f[i] - normMean[i] );
        }
    }

    for( int i = 0; i < vectorSize; i++ )
    {
        normStdv[i] = sqrt( normStdv[i] / ( vectors.size() - 1 ) );
        normAlpha[i] = log( 1 / epsilon - 1 ) / ( sigmaStep * normStdv[i] );
    }
}



void SigmoidNormalizer::normalize( DVectorPtr vector )
{
    QVector<double>& v = *vector.data();
    for( int i = 0; i < v.size(); i++ )
        v[i] = 1 / ( 1 + exp( -normAlpha[i] * ( v[i] - normMean[i] ) ) );
}



void SigmoidNormalizer::setFeature( DVectorPtr vector )
{
    QVector<double>& v = *vector.data();
    for( int i = 0; i < v.size(); i++ )
        v[i] = randomizer.randg( normMean[i], normStdv[i] );
}

} // namespace
