#pragma once

#include <QVector>

#include "grids/grid.hpp"
#include "grids/hexgrid.hpp"
#include "errors/somerror.h"

namespace somtk {

/// The Fast Hex Grid class provides an implementation of a wrapping 2d hexagonal grid
template <class T>
class WrapHexGrid : public HexGrid<T>
{

public:


    /// Constructs a hex grid with no size information
    WrapHexGrid() : HexGrid<T>(){}

    /// Constructs the hex grid with a specified size
    /// @see hexgrid.hpp for more documentation
    WrapHexGrid( QVector<int> size  )
        : HexGrid<T>()
    {
        this->init( size );
    }

    /// Destructs the HexGrid
    virtual ~WrapHexGrid(){}




    // Grid API - See grid.hpp for doumentation

    virtual QSharedPointer< Grid<T> > clone()
    {
        QSharedPointer< Grid<T> > newGrid( new WrapHexGrid<T>( this->size() ) );
        newGrid->setItems( this->items() );
        return newGrid;
    }

    virtual void checkSize( QVector<int> size )
    {
        SOMError::requireCondition(
                    size.size() == 1,
                    "Size vector must have exactly one element for a side of the grid"
                    );
        SOMError::requireCondition( size[0] > 0, "Grid side must be greater than 0" );
        SOMError::requireCondition( size[0] % 2 == 0, "Grid side must be even to facilitate wrapping" );
    }

    virtual int diagonal()
    {
        return ( this->s() - 1 ) * 2;
    }

    virtual int distance( int idx0, int idx1 )
    {
        QVector<int> coords0 = this->coords( idx0 );
        int x0 = coords0[0];
        int y0 = coords0[1];

        QVector<int> coords1 = this->coords( idx1 );
        int x1 = coords1[0];
        int y1 = coords1[1];

        int dx_norm = x1 - x0;
        int dy_norm = y1 - y0;

        int dx_wrap = dx_norm + ( dx_norm > 0 ? -this->s() : this->s() );
        int dy_wrap = dy_norm + ( dy_norm > 0 ? -this->s() : this->s() );

        int dist_norm_xy = qMax( qMax( abs(dx_norm), abs(dy_norm) ), abs( dy_norm + dx_norm ) );
        int dist_wrap_x  = qMax( qMax( abs(dx_wrap), abs(dy_norm) ), abs( dy_norm + dx_wrap ) );
        int dist_wrap_y  = qMax( qMax( abs(dx_norm), abs(dy_wrap) ), abs( dy_wrap + dx_norm ) );
        int dist_wrap_xy = qMax( qMax( abs(dx_wrap), abs(dy_wrap) ), abs( dy_wrap + dx_wrap ) );

        int distance = qMin( qMin( qMin( dist_norm_xy, dist_wrap_x ), dist_wrap_y ), dist_wrap_xy );

        return distance;
    }

    /// A fixed modulus operation....stupid c implementations
    /// @todo Move this into utilities
    inline int mod( int a, int n )
    {
        return ( a < 0 ) ? n - abs(a) % n : a % n;
    }

    virtual QVector<int> neighbors( int idx )
    {
        QVector<int> myCoords = this->coords( idx );
        int x = myCoords[0];
        int y = myCoords[1];

        int s = this->s();

        QVector<int> neighbors;
        neighbors << this->index( QVector<int>() <<               x << mod( y - 1, s ) );
        neighbors << this->index( QVector<int>() << mod( x + 1, s ) << mod( y - 1, s ) );
        neighbors << this->index( QVector<int>() << mod( x - 1, s ) <<               y );
        neighbors << this->index( QVector<int>() << mod( x + 1, s ) <<               y );
        neighbors << this->index( QVector<int>() << mod( x - 1, s ) << mod( y + 1, s ) );
        neighbors << this->index( QVector<int>() <<               x << mod( y + 1, s ) );

        return neighbors;
    }
};

} // namespace

