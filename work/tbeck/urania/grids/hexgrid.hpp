#pragma once

#include <QVector>
#include <QSize>
#include <QPoint>
#include <QPointF>

#include "grids/grid.hpp"
#include "errors/somerror.h"


namespace hsom {

/// Defines the vertical distance between cells in the grid
#define HG_B 0.86602540378443837

/** The HexGrid class provides the basis for the spatial organization of the Self Organizing Map.  It provides a
  * hexagonal grid which supports neighborhood searches, edge wrapping, and other functionality.
  */
template <class T>
class HexGrid : public Grid<T>
{

public:


    /// Constructs a hex grid with no size information
    HexGrid(){}

    /// Constructs the hex grid with a specified size
    HexGrid(
            QVector<int> size ///< The size of the new grid
            )
    {
        this->init( size );
    }

    /// Constructs the hex grid with a specified size
    HexGrid(
            int w, ///< The width of the hex grid
            int h  ///< The height of the hex grid
            )
    {
        QVector<int> size;
        size << w << h;
        this->init( size );
    }

    /// Constructs the hex grid with the specified size and fills it with the supplied values
    HexGrid(
            QVector<int> size, ///< The size of the new grid
            QVector<T> items   ///< The items with which to populate the grid
            )
    {
        this->init( size, items );
    }

    /// Constructs the hex grid with the specified size and fills it with the supplied values
    HexGrid(
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
    virtual ~HexGrid(){}



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
        SOMError::requireCondition( h % 2 == 0,
                                    "Hex Grid must have even height ( for edge wrapping )" );
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
        coords[1] = idx / this->_size[1];

        return coords;
    }

    virtual QVector<double> realBounds()
    {
        QVector<double> bounds;
        bounds << this->w() << this->h() * HG_B;
        return bounds;
    }

    virtual QVector<double> realCoords( int idx )
    {
        QVector<int> coords = this->coords( idx );
        int x = coords[0];
        int y = coords[1];
        QVector<double> realCoords( 2 );
        realCoords[0] = ( x + ( y % 2 ) * 0.5 );
        realCoords[1] = ( y * HG_B);
        return realCoords;
    }

    /// Fetches the length of the major diagonal running through the grid
    virtual int diagonal()
    {
        /// @todo  Come  up with a more precise computation of this
        return h() + w() - h() / 2;
    }

    /// Fetches indices for all slots within a radius of the specified slot
    virtual QVector< QPair<int, int> > neighborhood(
        int r,  ///< The radius within which a cell must lie to be a part of the neighborhood
        int idx ///< The origin index for the neighborhood ( center cell )
        )
    {
        /** @todo Consdier having the neighborhood function return a vector of pairs for index,distance
          *       instead of storing the distances in the grid
          */

        // Fetch the coordinate of the origin index
        QVector<int> origin_coords = coords( idx );
        int origin_x = origin_coords[0];
        int origin_y = origin_coords[1];

        // The neighbors vector will contain the indices of all cells in the neighborhood
        QVector< QPair<int, int> > globalNeighbors;

        // The local neighbors vector will contain all cells within the neighborhood discovered by each omp thread
        QVector< QPair<int, int> > localNeighbors;

        #pragma omp parallel
        {
            #pragma omp for private( localNeighbors )
            for( int i = 0; i < this->capacity(); i++ )
            {
                /* For each element in the loop, we will determine the direction and distance
                 * that we must step to get from the point represented by the index to the origin of
                 * the neighborhood
                 */

                // Fetch the coordinate of the current index
                QVector<int> pointCoords = coords( i );
                int point_x = pointCoords[0];
                int point_y = pointCoords[1];



                // Calculate the distance ( on the hex grid ) to the origin point
                int dist_y = abs( origin_y - point_y );

                // Calculate the distance if we were to move over the nearest y boudary
                int wrap_dist_y = this->_size[1] - dist_y;

                // If it is closer to wrap over the y boundary move that way
                if( wrap_dist_y < dist_y )
                    dist_y = wrap_dist_y;



                // Calculate the change in x to get from the point to the origin
                int delta_x = origin_x - point_x;

                // Calculate the distance between the origin and the point
                int dist_x = abs( delta_x );

                // Indicates if the point lies on an even row
                bool even_row   = !( point_y % 2 );

                // Indicates that the origin is to the right of the point
                bool move_right = delta_x >= 0;

                // Determine if the current movement gets an extra x for y movement
                int extra_x = move_right ^ even_row;

                // Calculate the distance if we were to move over the nearest x boundary
                int wrap_dist_x = this->_size[0] - dist_x;

                /* If it is closer to move over the x boundary, move that directon
                 * If it is exactly the same distance, wrap only if the current direction
                 * does not get an extra x for y movement
                 */
                if( ( wrap_dist_x  < dist_x ) || ( wrap_dist_x == dist_x && !extra_x ) )
                {
                    dist_x  = wrap_dist_x;
                    extra_x = !extra_x;
                }

                // Account for the number of free x steps that can be gained by moving in  y
                int free_x = ( dist_y + extra_x ) / 2;

                // Update the x distance by accounting for free x steps
                dist_x = std::max( dist_x - free_x, 0 );

                // Calculate the distance between the points
                int dist = dist_y + dist_x;

                // If the distance is within the radius, add this index to the local neighbors
                if( dist < r )
                    localNeighbors.append( QPair<int, int>( i, dist ) );
            }

            // Each thread adds it's local neighbor list to the global neighbor list
            #pragma omp critical
            {
                globalNeighbors += localNeighbors;
            }
        }
        return globalNeighbors;
    }

};

} // namespace hsom
