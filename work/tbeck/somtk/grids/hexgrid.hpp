#pragma once

#include <QVector>

#include "grids/grid.hpp"
#include "errors/somerror.h"

namespace somtk {

/// Defines the vertical distance between cells in the grid
#define HG_B 0.86602540378443837

/// The Fast Hex Grid class provides an implementation of a fast, non-wrapping 2d hexagonal grid
template <class T>
class HexGrid : public Grid<T>
{

public:

    /// Constructs a grid with no size information
    HexGrid() : Grid<T>(){}

    /// Destructs the grid
    virtual ~HexGrid(){}



    // Convenience functions for using QPoints and QSizes

    int s()
    {
        return this->_size[0];
    }



    // Grid API - See grid.hpp for doumentation

    virtual int capacityFromSize( QVector<int> size )
    {
        this->checkSize( size );
        return size[0] * size[0];
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
        QVector<double> bounds;
        bounds << this->s() + this->s() / 2.0;
        bounds << this->s() * HG_B;
        return bounds;
    }

    virtual QVector<double> realCoords( int idx )
    {
        QVector<int> coords = this->coords( idx );
        int x = coords[0];
        int y = coords[1];

        QVector<double> realCoords( 2 );
        realCoords[0] = x + 0.5 * y;
        realCoords[1] = y * HG_B;
        return realCoords;
    }
};

} // namespace

