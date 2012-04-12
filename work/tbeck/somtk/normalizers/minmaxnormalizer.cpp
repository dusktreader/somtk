#include "minmaxnormalizer.h"

namespace somtk {

MinMaxNormalizer::MinMaxNormalizer() :
    Normalizer()
{}



void MinMaxNormalizer::calculateNormalizer( QVector<FeaturePtr> features, QMap<QString, QVariant> normalizerParameters )
{
    bool ok = true;

    ASSERT_MSG( normalizerParameters.contains( "HiLimit" ), "Normalizer parameters do not include a high value limit" );
    hiLimit = normalizerParameters[ "HiLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert high limit parameter" );

    ASSERT_MSG( normalizerParameters.contains( "LoLimit" ), "Normalizer parameters do not include a low value limit" );
    loLimit = normalizerParameters[ "LoLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert low limit parameter" );
    ASSERT_MSG( loLimit < hiLimit, "Low limit must be less than high limit" );

    int featureSize = features.first()->size();

    los = QVector<double>( featureSize, DBL_MAX );
    his = QVector<double>( featureSize, DBL_MIN );

    foreach( FeaturePtr feature, features )
    {
        Feature& f = *feature.data();
        for( int i = 0; i < featureSize; i++ )
        {
            los[i] = qMin( f[i], los[i] );
            his[i] = qMax( f[i], his[i] );
        }
    }
}



void MinMaxNormalizer::normalize( FeaturePtr feature )
{
    Feature& f = *feature.data();
    for( int i = 0; i < f.size(); i++ )
    {
        f[i] = qMin( f[i], his[i] );
        f[i] = qMax( f[i], los[i] );
        f[i] = ( f[i] - los[i] ) / ( his[i] - los[i] ) * ( hiLimit - loLimit ) + loLimit;
    }
}



void MinMaxNormalizer::setFeature( FeaturePtr feature )
{
    Feature& f = *feature.data();
    for( int i = 0; i < f.size(); i++ )
        f[i] = randomizer.randd( loLimit, hiLimit );
}

} // namespace hsom
