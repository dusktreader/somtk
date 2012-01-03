#include "histogram.h"

namespace hsom {

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

/*
cv::Mat Histogram::visualize()
{
    double minVal = grid[0];
    double maxVal = grid[0];
    for( int i=0; i<grid.l(); i++ )
    {
        minVal = min( minVal, grid[i] );
        maxVal = max( maxVal, grid[i] );
    }

    int radius = 10;
    double buff = 2.5 * radius;
    SizePlus<int> gridSz = grid.size();
    SizePlus<int> vizSz( (int)( 0.5 * buff + buff * gridSz.w ), (int)( buff * gridSz.h * HG_B ) );
    cv::Mat viz( vizSz, 0 );
    viz.setTo( 0 );
    PointPlus<double> pt;
    for( int i=0; i<grid.l(); i++ )
    {
        pt = grid.realCoords( grid.coords(i) );
        pt.x = pt.x * buff + buff / 2.0;
        pt.y = pt.y * buff + buff / 2.0;
        cv::circle( viz, pt, radius, cv::Scalar( ( grid[i] - minVal ) / maxVal * 255 ), CV_FILLED );
    }
    return viz;
}
*/

} // namespace hsom
