#include "minmaxnormalizer.h"

namespace hsom {

MinMaxNormalizer::MinMaxNormalizer() :
    Normalizer()
{}



void MinMaxNormalizer::calculateNormalizer( QVector<FeaturePtr> features, QMap<QString, QVariant> nomalizerParameters )
{
    bool ok = true;

    ASSERT_MSG( normalizerParameters.contains( "HiLimit" ), "Normalizer parameters do not include a high value limit" );
    hiLimit = normalizerParameters[ "HiLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert high limit parameter" );

    ASSERT_MSG( normalizerParameters.contains( "LoLimit" ), "Normalizer parameters do not include a low value limit" );
    loLimit = normalizerParameters[ "LoLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert low limit parameter" );
    ASSERT_MSG( loLimit < hiLimit, "Low limit must be less than high limit" );

    unsigned featureSize = features.first().size();

    los = QVector<double>( featureSize, DBL_MAX );
    his = QVector<double>( featureSize, DBL_MIN );

    foreach( Feature feature, features )
    {
        for( int i = 0; i < featureSize; i++ )
        {
            los[i] = qMin( feature[i], los[i] );
            his[i] = qMax( feature[i], his[i] );
        }
    }
}



void MinMaxNormalizer::normalize( Feature& feature )
{
    for( int i = 0; i < feature.size(); i++ )
    {
        if( feature[i] > his[i] )
            feature[i] = his[i];
        else if( feature[i] < los[i] )
            feature[i] = los[i];
        feature[i] = ( feature[i] - los[i] ) / ( his[i] - los[i] ) * ( hiLimit - loLimit ) + loLimit;
    }
}



void MinMaxNormalizer::set( Feature& feature )
{
    for( int i = 0; i < feature.size(); i++ )
        feature[i] = randomizer.randd( loLimit, hiLimit );
}

} // namespace hsom
