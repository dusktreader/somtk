#include "feature.h"

namespace hsom {

Feature::Feature() :
    QVector<double>()
{}



Feature::Feature( int size ) :
    QVector<double>( size )
{}



Feature::Feature( QVector<double> other ) :
    QVector<double>( other )
{}



double Feature::distance( Feature other ) const
{
    /// @todo:  make sure that the two features are the same size!
    double d = 0.0;
    for( int i = 0; i < size(); i++ )
        d += pow( *this[i] - other[i], 2 );
    return d;
}



void Feature::adjust( Feature other, double scaleFactor )
{
    for( int i = 0; i < size(); i++ )
        *this[i] = ( other[i] - *this[i] ) * scaleFactor;
}

} // namespace hsom
