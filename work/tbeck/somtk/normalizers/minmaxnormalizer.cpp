#include "minmaxnormalizer.h"

namespace somtk {

MinMaxNormalizer::MinMaxNormalizer() : Normalizer(){}



void MinMaxNormalizer::calculateNormalizer( QVector<DVectorPtr> vectors, QMap<QString, QVariant> normalizerParameters )
{
    bool ok = true;

    ASSERT_MSG( normalizerParameters.contains( "HiLimit" ), "Normalizer parameters do not include a high value limit" );
    hiLimit = normalizerParameters[ "HiLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert high limit parameter" );

    ASSERT_MSG( normalizerParameters.contains( "LoLimit" ), "Normalizer parameters do not include a low value limit" );
    loLimit = normalizerParameters[ "LoLimit" ].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert low limit parameter" );
    ASSERT_MSG( loLimit < hiLimit, "Low limit must be less than high limit" );

    int vectorSize = vectors.first()->size();

    los = QVector<double>( vectorSize, DBL_MAX );
    his = QVector<double>( vectorSize, DBL_MIN );

    foreach( DVectorPtr vector, vectors )
    {
        QVector<double>& v = *vector.data();
        for( int i = 0; i < vectorSize; i++ )
        {
            los[i] = qMin( v[i], los[i] );
            his[i] = qMax( v[i], his[i] );
        }
    }
}



void MinMaxNormalizer::normalize( DVectorPtr vector )
{
    QVector<double>& v = *vector.data();
    for( int i = 0; i < v.size(); i++ )
    {
        v[i] = qMin( v[i], his[i] );
        v[i] = qMax( v[i], los[i] );
        v[i] = ( v[i] - los[i] ) / ( his[i] - los[i] ) * ( hiLimit - loLimit ) + loLimit;
    }
}



void MinMaxNormalizer::setFeature( DVectorPtr vector )
{
    QVector<double>& v = *vector.data();
    for( int i = 0; i < v.size(); i++ )
        v[i] = randomizer.randd( loLimit, hiLimit );
}

} // namespace
