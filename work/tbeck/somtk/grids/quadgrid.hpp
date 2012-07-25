#pragma once

#include <QVector>
#include <QPointF>

#include "grids/grid.hpp"
#include "errors/somerror.h"


namespace somtk {

/// Defines the vertical distance between cells in the grid
#define HG_B 0.86602540378443837

/** The HexGrid class provides the basis for the spatial organization of the Self Organizing Map.  It provides a
  * hexagonal grid which supports neighborhood searches, edge wrapping, and other functionality.
  */
template <class T>
class QuadGrid : public Grid<T>
{

public:


    /// Constructs a hex grid with no size information
    QuadGrid(){}

    /// Constructs the hex grid with a specified size
    QuadGrid(
            QVector<int> size ///< The size of the new grid
            )
    {
        this->init( size );
    }

    /// Constructs the hex grid with a specified size
    QuadGrid(
            int w, ///< The width of the hex grid
            int h  ///< The height of the hex grid
            )
    {
        QVector<int> size;
        size << w << h;
        this->init( size );
    }

    /// Constructs the hex grid with the specified size and fills it with the supplied values
    QuadGrid(
            QVector<int> size, ///< The size of the new grid
            QVector<T> items   ///< The items with which to populate the grid
            )
    {
        this->init( size, items );
    }

    /// Constructs the hex grid with the specified size and fills it with the supplied values
    QuadGrid(
            int w,             ///< The width of the hex grid
            int h,             ///< The height of the hex grid
            QVector<T> items   ///< The items with which to populate the grid
            )
    {
        QVector<int> size;
        size << w << h;
        this->init( size, items );
    }

    /// Destructs the HexGrid
    virtual ~QuadGrid(){}



    // Convenience functions for using QPoints and QSizes

    int w()
    {
        return this->_size[0];
    }

    int h()
    {
        return this->_size[1];
    }





    // Grid API

    virtual QSharedPointer< Grid<T> > clone()
    {
        QSharedPointer< Grid<T> > newGrid( new QuadGrid<T>( this->size(), this->items() ) );
        return newGrid;
    }

    /// Checks the supplied size to ensure that it is valid for the HexGrid
    virtual void checkSize( QVector<int> size )
    {
        SOMError::requireCondition( size.size() == 2,
                                    "Size vector must have exactly two elements for width and height" );
        int w = size[0];
        int h = size[1];

        SOMError::requireCondition( w > 0,
                                    "Grid width must be greater than 0" );
        SOMError::requireCondition( h > 0,
                                    "Grid height must be greater than 0" );
    }

    virtual int capacityFromSize( QVector<int> size )
    {
        checkSize( size );
        int w = size[0];
        int h = size[1];
        return w * h;
    }

    virtual void checkCoords( QVector<int> coords )
    {
        SOMError::requireCondition( coords.size() == 2,
                                    "Coords vector must have exactly two elements for x and y" );
        int x = coords[0];
        int y = coords[1];

        SOMError::requireCondition( x >= 0 && y < w(),
                                    QString( "x coordinate must be in the range [ 0 , %1 ): was %2" )
                                    .arg( w() ).arg( x ) );

        SOMError::requireCondition( y >= 0 && y < h(),
                                    QString( "y coordinate must be in the range [ 0 , %1 ): was %2" )
                                    .arg( h() ).arg( y ) );
    }

    /// Fetches the index for the slot at the specified coordinates
    virtual int index(
            QVector<int> coords ///< The point in the grid for which to fetch the index
            )
    {
        checkCoords( coords );
        int x = coords[0];
        int y = coords[1];
        return y * w() + x;
    }

    /// Fetches the coordinates given an index
    virtual QVector<int> coords(
        int idx ///< The index in the grid for which to fetch the coordinates
        )
    {
        QVector<int> coords( 2 );
        coords[0] = idx % this->_size[0];
        coords[1] = idx / this->_size[0];

        return coords;
    }

    virtual QVector<double> realBounds()
    {
        QVector<double> bounds;
        bounds << this->w() << this->h();
        return bounds;
    }

    virtual QVector<double> realCoords( int idx )
    {
        QVector<int> coords = this->coords( idx );
        int x = coords[0];
        int y = coords[1];
        QVector<double> realCoords( 2 );
        realCoords[0] = ( x );
        realCoords[1] = ( y );
        return realCoords;
    }

    /// Fetches the length of the major diagonal running through the grid
    virtual int diagonal()
    {
        /// @todo  Come  up with a more precise computation of this
        return h() + w();
    }

    /// Fetches indices for all slots within a radius of the specified slot
    virtual int distance( int idx0, int idx1 )
    {
        QVector<int> coords0 = coords( idx0 );
        int x0 = coords0[0];
        int y0 = coords0[1];

        QVector<int> coords1 = coords( idx1 );
        int x1 = coords1[0];
        int y1 = coords1[1];

        // Calculate the distance ( on the hex grid ) to the origin point
        int dist_y = abs( y0 - y1 );

        // Calculate the distance if we were to move over the nearest y boudary
        int wrap_dist_y = this->_size[1] - dist_y;

        // If it is closer to wrap over the y boundary move that way
        if( wrap_dist_y < dist_y )
            dist_y = wrap_dist_y;

        // Calculate the distance ( on the hex grid ) to the origin point
        int dist_x = abs( x0 - x1 );

        // Calculate the distance if we were to move over the nearest y boudary
        int wrap_dist_x = this->_size[0] - dist_x;

        // If it is closer to wrap over the y boundary move that way
        if( wrap_dist_x < dist_x )
            dist_x = wrap_dist_x;

        // Calculate the distance between the points
        int distance = dist_y + dist_x;

        return distance;
    }



    virtual QVector<int> neighbors( int idx )
    {
        QVector<int> myCoords = coords( idx );
        int x = myCoords[0];
        int y = myCoords[1];

        QVector<int> neighbors;

        neighbors[0][0] = ( x - 1 ) % s();
        neighbors[0][1] = ( y - 1 ) % s();

        neighbors[0][0] =               x;
        neighbors[0][1] = ( y - 1 ) % s();

        neighbors[0][0] = ( x + 1 ) % s();
        neighbors[0][1] = ( y - 1 ) % s();



        neighbors[0][0] = ( x - 1 ) % s();
        neighbors[0][1] =               y;

        neighbors[0][0] = ( x + 1 ) % s();
        neighbors[0][1] =               y;



        neighbors[0][0] = ( x - 1 ) % s();
        neighbors[0][1] = ( y + 1 ) % s();

        neighbors[0][0] =               x;
        neighbors[0][1] = ( y + 1 ) % s();

        neighbors[0][0] = ( x + 1 ) % s();
        neighbors[0][1] = ( y + 1 ) % s();

        return neighbors;
    }

};

} // namespace
