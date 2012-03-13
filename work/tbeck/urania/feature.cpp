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
        d += pow( (*this)[i] - other[i], 2.0 );
    return d;
}



void Feature::adjust( Feature other, double scaleFactor )
{
    std::ofstream out( "features.txt", std::ios_base::app );
    out << "scale factor: " << scaleFactor << std::endl;
    out << "other:" << other[0] << " " << other[1] << " " << other[2] << std::endl;
    out << "before adjust:" << (*this)[0] << " " << (*this)[1] << " " << (*this)[2] << std::endl;

    double adj[other.size()];
    for( int i = 0; i < size(); i++ )
    {
        adj[i] = ( other[i] - (*this)[i] ) * scaleFactor;
        (*this)[i] += adj[i];
    }
    out << "adjust vector:" << adj[0] << " " << adj[1] << " " << adj[2] << std::endl;
    out << "after adjust:" << (*this)[0] << " " << (*this)[1] << " " << (*this)[2] << std::endl;
}

} // namespace hsom
