#include "minmaxnormalizer.h"

namespace somtk {

MinMaxNormalizer::MinMaxNormalizer() : Normalizer(){}



void MinMaxNormalizer::calculateNormalizer( QVector<FeaturePtr> features )
{
    bool ok = true;

    ASSERT_MSG( _calculationParameters.contains( "HiLimit" ), "Normalizer parameters do not include a high value limit" );
    hiLimit = _calculationParameters[ "HiLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert high limit parameter" );

    ASSERT_MSG( _calculationParameters.contains( "LoLimit" ), "Normalizer parameters do not include a low value limit" );
    loLimit = _calculationParameters[ "LoLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert low limit parameter" );
    ASSERT_MSG( loLimit < hiLimit, "Low limit must be less than high limit" );

    int featureSize = features.first()->size();

    los = QVector<double>( featureSize, DBL_MAX );
    his = QVector<double>( featureSize, DBL_MIN );

    foreach( FeaturePtr feature, features )
    {
        QVector<double>& v = *feature.data();
        for( int i = 0; i < featureSize; i++ )
        {
            los[i] = qMin( v[i], los[i] );
            his[i] = qMax( v[i], his[i] );
        }
    }
}



void MinMaxNormalizer::normalize( FeaturePtr feature )
{
    QVector<double>& v = *feature.data();
    for( int i = 0; i < v.size(); i++ )
    {
        v[i] = qMin( v[i], his[i] );
        v[i] = qMax( v[i], los[i] );
        v[i] = ( v[i] - los[i] ) / ( his[i] - los[i] ) * ( hiLimit - loLimit ) + loLimit;
    }
}



void MinMaxNormalizer::setFeature( FeaturePtr feature )
{
    QVector<double>& v = *feature.data();
    for( int i = 0; i < v.size(); i++ )
        v[i] = randomizer.randd( loLimit, hiLimit );
}

} // namespace
