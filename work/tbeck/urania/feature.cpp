#include "feature.h"

namespace hsom {

RandMaster Feature::rng;

Feature::Feature() :
    QVector<double>()
{}

/// @todo:  Add a constructor to initialize the feature with a function?

Feature::Feature( const QVector<double>& other ) :
    QVector<double>( other )
{}

Feature::~Feature(){}

double Feature::distance( const FeaturePtr other ) const
{
    /// @todo:  make sure that the two features are the same size!
    double d = 0.0;
    for( int i = 0; i < size(); i++ )
        d += pow( at( i ) - other->at( i ), 2 );
    return d;
}

void Feature::adjust( const FeaturePtr other, double scaleFactor )
{
    for( int i = 0; i < size(); i++ )
        *this[i] = ( other->at( i ) - at( i ) ) * scaleFactor;
}

void Feature::initialize()
{
    for( int i = 0; i < size(); i++ )
        *this[i] = rng.randd();
}

} // namespace hsoms
