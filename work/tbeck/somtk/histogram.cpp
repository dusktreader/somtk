#include "histogram.h"

namespace somtk {

Histogram::Histogram() :
    HexGrid()
{}

Histogram::Histogram( QSize size ) :
    HexGrid( size )
{}

Histogram::~Histogram()
{}

double Histogram::bin( int idx )
{
    return grid[idx];
}

double Histogram::bin( QPoint point )
{
    return grid[point];
}

void Histogram::reset()
{
    setTo( 0.0 );
}

void Histogram::increment( QPoint point )
{
    grid[point]++;
}

void Histogram::increment( int idx )
{
    grid[idx]++;
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

}
