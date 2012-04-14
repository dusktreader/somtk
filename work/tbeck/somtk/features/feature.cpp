#include "feature.h"

namespace somtk {

Feature::Feature() : QVector<double>(){}

Feature::Feature( int size ) : QVector<double>( size ){}

Feature::Feature( QVector<double> other ) : QVector<double>( other ){}



double Feature::distance( Feature a, Feature b )
{
    /// @todo:  make sure that the two features are the same size!
    double d = 0.0;
    for( int i = 0; i < a.size(); i++ )
        d += pow( a[i] - b[i], 2.0 );
    return d;
}

double Feature::distance( Feature other ) const
{
    return distance( *this, other );
}

double Feature::distance( FeaturePtr other ) const
{
    return distance( *this, *other.data() );
}



void Feature::adjust( Feature& a, Feature b, double scaleFactor )
{
    for( int i = 0; i < a.size(); i++ )
        a[i] += ( b[i] - a[i] ) * scaleFactor;
}

void Feature::adjust( Feature other, double scaleFactor )
{
    adjust( *this, other, scaleFactor );
}

void Feature::adjust( FeaturePtr other, double scaleFactor )
{
    adjust( *this, *other.data(), scaleFactor );
}

} // namespace
