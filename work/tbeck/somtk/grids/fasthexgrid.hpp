#pragma once

#include <QVector>

#include "grids/grid.hpp"
#include "grids/hexgrid.hpp"
#include "errors/somerror.h"

#include <iostream>

namespace somtk {

/// The Fast Hex Grid class provides an implementation of a fast, non-wrapping 2d hexagonal grid
template <class T>
class FastHexGrid : public HexGrid<T>
{

public:

    /// Constructs a grid with no size information
    FastHexGrid() : HexGrid<T>(){}

    /// Constructs the grid with a specified size
    /// @see hexgrid.hpp for more documentation
    FastHexGrid( QVector<int> size  )
        : HexGrid<T>()
    {
        this->init( size );
    }

    /// Destructs the grid
    virtual ~FastHexGrid(){}



    // Grid API - See grid.hpp for doumentation

    virtual QSharedPointer< Grid<T> > clone()
    {
        QSharedPointer< Grid<T> > newGrid( new FastHexGrid<T>( this->size() ) );
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

        int dx = x1 - x0;
        int dy = y1 - y0;

        int distance = std::max( std::max( abs(dx), abs(dy) ), abs( dy + dx ) );

        return distance;
    }

    virtual QVector<int> neighbors( int idx )
    {
        QVector<int> myCoords = this->coords( idx );
        int x = myCoords[0];
        int y = myCoords[1];
        int s = this->s();

        QVector<int> neighbors;

        if( x - 1 >= 0 )
            neighbors << this->index( QVector<int>() << x - 1 <<     y );

        if( x - 1 >= 0 && y + 1 < s )
            neighbors << this->index( QVector<int>() << x - 1 << y + 1 );

        if( x + 1 < s )
            neighbors << this->index( QVector<int>() << x + 1 <<     y );

        if( x + 1 < s && y - 1 >= 0 )
            neighbors << this->index( QVector<int>() << x + 1 << y - 1 );

        if( y - 1 >= 0 )
            neighbors << this->index( QVector<int>() <<     x << y - 1 );

        if( y + 1 < s )
            neighbors << this->index( QVector<int>() <<     x << y + 1 );

        return neighbors;
    }
};

} // namespace

