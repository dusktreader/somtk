#include "minmaxnormalizer.h"

namespace somtk {

MinMaxNormalizer::MinMaxNormalizer() : Normalizer(){}



void MinMaxNormalizer::initialize( QMap<QString, QVariant> nomalizerParameters )
{
    bool ok = true;

    ASSERT_MSG( nomalizerParameters.contains( "HiLimit" ), "Normalizer parameters do not include a high value limit" );
    hiLimit = nomalizerParameters[ "HiLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert high limit parameter" );

    ASSERT_MSG( nomalizerParameters.contains( "LoLimit" ), "Normalizer parameters do not include a low value limit" );
    loLimit = nomalizerParameters[ "LoLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert low limit parameter" );
    ASSERT_MSG( loLimit < hiLimit, "Low limit must be less than high limit" );
}



void MinMaxNormalizer::calculate( QVector<FeaturePtr> features )
{
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



void MinMaxNormalizer::set( FeaturePtr feature )
{
    QVector<double>& v = *feature.data();
    for( int i = 0; i < v.size(); i++ )
        v[i] = randomizer.randd( loLimit, hiLimit );
}

} // namespace
