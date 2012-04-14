#include "histogram.h"

namespace somtk {

Histogram::Histogram( Grid&<double> grid ) : _grid( grid ){}

virtual Histogram::~Histogram(){}

double Histogram::bin( int idx )
{
    return _grid[idx];
}

void Histogram::reset()
{
    _grid.setTo( 0.0 );
}

void Histogram::increment( int idx )
{
    _grid[idx]++;
}

void Histogram::normalize()
{
    /// @todo  investigate other normalizations.
    double minVal = bin( 0 );
    double maxVal = minVal;;
    for( int i=1; i<grid.l(); i++ )
    {
        minVal = min( bin( i ), minVal );
        maxVal = max( bin( i ), maxVal );
    }
    for( int i=0; i<grid.l(); i++ )
        bin( i ) = ( bin( i ) - minVal ) / maxVal;
}

} // namespace
