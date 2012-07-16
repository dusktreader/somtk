#pragma once

#include <QVector>

#include <iostream>

#include "grids/grid.hpp"
#include "grids/fasthexgrid.hpp"
#include "errors/somerror.h"


namespace somtk {

/// Defines the vertical distance between cells in the grid
#define HG_B 0.86602540378443837

/** The HexGrid class provides the basis for the spatial organization of the Self Organizing Map.  It provides a
  * hexagonal grid which supports neighborhood searches, edge wrapping, and other functionality.
  */
template <class T>
class WrapHexGrid : public FastHexGrid<T>
{

public:


    /// Constructs a hex grid with no size information
    WrapHexGrid() : FastHexGrid<T>(){}

    /// Constructs the hex grid with a specified size
    WrapHexGrid(
            QVector<int> size ///< The size of the new grid
            )
        : FastHexGrid<T>(size){}

    /// Constructs the hex grid with a specified size
    WrapHexGrid(
            int s ///< The length of one side of the grid
            )
        : FastHexGrid<T>(s){}

    /// Constructs the hex grid with the specified size and fills it with the supplied values
    WrapHexGrid(
            QVector<int> size, ///< The size of the new grid
            QVector<T> items   ///< The items with which to populate the grid
            )
        : FastHexGrid<T>(size,items){}

    /// Constructs the hex grid with the specified size and fills it with the supplied values
    WrapHexGrid(
            int s,           ///< The length of one side of the grid
            QVector<T> items ///< The items with which to populate the grid
            )
        : FastHexGrid<T>( s, items ){}

    /// Destructs the HexGrid
    virtual ~WrapHexGrid(){}




    // Grid API - See grid.hpp for doumentation

    virtual QSharedPointer< Grid<T> > clone()
    {
        QSharedPointer< Grid<T> > newGrid( new WrapHexGrid<T>( this->size(), this->items() ) );
        return newGrid;
    }

    virtual void checkSize( QVector<int> size )
    {
        SOMError::requireCondition( size.size() == 1,
                                    "Size vector must have exactly one element for a side of the grid" );
        int s = size[0];

        SOMError::requireCondition( s % 2 == 0,
                                    "Grid side must be even to facilitate wrapping" );

        FastHexGrid<T>::checkSize( size );
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



};

} // namespace

