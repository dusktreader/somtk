#include "somhistogram.h"

using namespace std;

SOMHistogram::SOMHistogram(){}

SOMHistogram::SOMHistogram( const SizePlus<int>& sz)
{
    grid = HexGrid<double>( sz );
}

SOMHistogram::~SOMHistogram(){}

void SOMHistogram::reset()
{
    grid.setTo( 0.0 );
}

void SOMHistogram::increment( const PointPlus<int>& pt )
{
    grid[pt]++;
}

void SOMHistogram::increment( int idx )
{
    grid[idx]++;
}

void SOMHistogram::normalize()
{
    /// @todo  investigate other normalizations.
    double minVal = grid[0];
    double maxVal = grid[0];
    for( int i=1; i<grid.l(); i++ )
    {
        minVal = min( grid[i], minVal );
        maxVal = max( grid[i], maxVal );
    }
    for( int i=0; i<grid.l(); i++ )
        grid[i] = ( grid[i] - minVal ) / maxVal;
}

double SOMHistogram::bin( int idx )
{
    return grid[idx];
}

double SOMHistogram::bin( const PointPlus<int>& pt )
{
    return grid[pt];
}

cv::Mat SOMHistogram::vizHistogram()
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
    mapSz = SizePlus<int>( (int)( 0.5 * buff + buff * w ), (int)( buff * h * HG_B ) );
    cv::Mat map( mapSz );
    map.setTo( 0 );
    PointPlus<double> pt;
    for( int i=0; i<l; i++ )
    {
        pt = grid.realCoords( grid.coords(i) );
        pt.x = pt.x * buff + buff / 2.0;
        pt.y = pt.y * buff + buff / 2.0;
        cv::circle( map, pt, radius, cv::Scalar( ( grid[i] - minVal ) / maxVal * 255 ), CV_FILLED );
    }
    return map;
}
