#pragma once

#include <QVector>

#include "grids/grid.hpp"
#include "errors/somerror.h"


namespace hsom {

/// Defines the vertical distance between cells in the grid
#define HG_B 0.86602540378443837

/** The HexGrid class provides the basis for the spatial organization of the Self Organizing Map.  It provides a
  * hexagonal grid which supports neighborhood searches, edge wrapping, and other functionality.
  */
template <class T>
class FastHexGrid : public Grid<T>
{

public:


    /// Constructs a hex grid with no size information
    FastHexGrid(){}

    /// Constructs the hex grid with a specified size
    FastHexGrid(
            QVector<int> size ///< The size of the new grid
            )
    {
        this->init( size );
    }

    /// Constructs the hex grid with a specified size
    FastHexGrid(
            int s ///< The length of one side of the grid
            )
    {
        QVector<int> size;
        size << s;
        this->init( size );
    }

    /// Constructs the hex grid with the specified size and fills it with the supplied values
    FastHexGrid(
            QVector<int> size, ///< The size of the new grid
            QVector<T> items   ///< The items with which to populate the grid
            )
    {
        this->init( size, items );
    }

    /// Constructs the hex grid with the specified size and fills it with the supplied values
    FastHexGrid(
            int s,           ///< The length of one side of the grid
            QVector<T> items ///< The items with which to populate the grid
            )
    {
        QVector<int> size;
        size << s;
        this->init( size, items );
    }

    /// Destructs the HexGrid
    virtual ~FastHexGrid(){}



    // Convenience functions for using QPoints and QSizes

    int s()
    {
        return this->_size[0];
    }



    // Grid API - See grid.hpp for doumentation

    virtual void checkSize( QVector<int> size )
    {
        SOMError::requireCondition( size.size() == 1,
                                    "Size vector must have exactly one element for a side of the grid" );
        int s = size[0];

        SOMError::requireCondition( s > 0,
                                    "Grid side must be greater than 0" );
    }

    virtual int capacityFromSize( QVector<int> size )
    {
        checkSize( size );
        int s = size[0];
        return s * s;
    }

    virtual void checkCoords( QVector<int> coords )
    {
        SOMError::requireCondition( coords.size() == 2,
                                    "Coords vector must have exactly two elements for x and y" );
        int x = coords[0];
        int y = coords[1];

        SOMError::requireCondition( x >= 0 && x < s(),
                                    QString( "x coordinate must be in the range [ 0 , %1 ): was %2" )
                                    .arg( this->s() ).arg( x ) );

        SOMError::requireCondition( y >= 0 && y < s(),
                                    QString( "y coordinate must be in the range [ 0 , %1 ): was %2" )
                                    .arg( this->s() ).arg( y ) );
    }

    virtual int index(
            QVector<int> coords ///< The point in the grid for which to fetch the index
            )
    {
        checkCoords( coords );
        int x = coords[0];
        int y = coords[1];
        return y * s() + x;
    }

    virtual QVector<int> coords(
        int idx ///< The index in the grid for which to fetch the coordinates
        )
    {
        QVector<int> coords( 2 );
        coords[0] = idx % this->s();
        coords[1] = idx / this->s();

        return coords;
    }

    virtual QVector<double> realBounds()
    {
        QVector<double> bounds( 2 );
        bounds << 2 * this->s() * 0.5;
        bounds << 2 * this->s() * HG_B;
        return bounds;
    }

    virtual QVector<double> realCoords( int idx )
    {
        double offset = HG_B * this->s();
        QVector<int> coords = this->coords( idx );
        int x = coords[0];
        int y = coords[1];
        QVector<double> realCoords( 2 );
        realCoords[0] = ( y + x ) *  0.5 + offset;
        realCoords[1] = ( y - x ) * HG_B;
        return realCoords;
    }

    virtual int diagonal()
    {
        /// @todo  Come  up with a more precise computation of this
        return this->s();
    }

    virtual int distance( int idx0, int idx1 )
    {
        QVector<int> coords0 = coords( idx0 );
        int x0 = coords0[0];
        int y0 = coords0[1];

        QVector<int> coords1 = coords( idx1 );
        int x1 = coords1[0];
        int y1 = coords1[1];

        int dx = x1 - x0;
        int dy = y1 - y0;
        int norm_distance = std::max( std::max( abs(dx), abs(dy) ), abs( dy - dx ) );

        dx += dx > 0 ? -this->s() : this->s();
        dy += dy > 0 ? -this->s() : this->s();
        int wrap_distance = std::max( std::max( abs(dx), abs(dy) ), abs( dy - dx ) );

        return std::min( norm_distance, wrap_distance );
    }



};

} // namespace hsom

