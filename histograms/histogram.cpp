#include "histogram.h"

namespace somtk {

Histogram::Histogram( HistogramGrid gridTemplate ) : _grid( grid->clone() ){}

Histogram::~Histogram(){}

void Histogram::reset()
{
    _grid->setTo( 0.0 );
}

void Histogram::increment( int idx )
{
    _grid->item( idx )++;
}

double Histogram::bin( int idx )
{
    return _grid->item( idx );
}

void Histogram::normalize()
{
    /// @todo  investigate other normalizations.
    double minVal = _grid->item( 0 );
    double maxVal = minVal;
    for( int i = 1; i < _grid->capacity(); i++ )
    {
        minVal = qMin( _grid->item( i ), minVal );
        maxVal = qMax( _grid->item( i ), maxVal );
    }

    #pragma omp parallel for
    for( int i = 0; i < _grid->capacity(); i++ )
        _grid->item( i ) = ( _grid->item( i ) - minVal ) / maxVal;
}

QColor Histogram::render( double item )
{
    int value = (int)( item * 255 );
    return QColor( value, value, value );
}

QImage Histogram::visualize()
{
    return _grid->visualize( 10, &render );
}

} // namespace
