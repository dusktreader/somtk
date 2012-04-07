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
            int s ///< The length of one side of the grid
            )
    {
        QVector<int> size;
        size << s;
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
            int s,           ///< The length of one side of the grid
            QVector<T> items ///< The items with which to populate the grid
            )
    {
        QVector<int> size;
        size << s;
        this->init( size, items );
    }

    /// Destructs the HexGrid
    virtual ~HexGrid(){}



    // Convenience functions for using QPoints and QSizes

    int s()
    {
        return this->_size[0];
    }



    // Grid API

    /// Checks the supplied size to ensure that it is valid for the HexGrid
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

    /// Fetches the index for the slot at the specified coordinates
    virtual int index(
            QVector<int> coords ///< The point in the grid for which to fetch the index
            )
    {
        checkCoords( coords );
        int x = coords[0];
        int y = coords[1];
        return y * s() + x;
    }

    /// Fetches the coordinates given an index
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

    /// Fetches the length of the major diagonal running through the grid
    virtual int diagonal()
    {
        /// @todo  Come  up with a more precise computation of this
        return this->s();
    }

    /// Fetches indices for all slots within a radius of the specified slot
    virtual QVector< QPair<int, int> > neighborhood(
        int r,  ///< The radius within which a cell must lie to be a part of the neighborhood
        int idx ///< The origin index for the neighborhood ( center cell )
        )
    {

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
                QVector<int> pointCoords = coords( i );
                int point_x = pointCoords[0];
                int point_y = pointCoords[1];

                int dx_norm = point_x - origin_x;
                int dx_wrap = dx_norm + ( dx_norm > 0 ? -this->s() : this->s() );

                int dy_norm = point_y - origin_y;
                int dy_wrap = dy_norm + ( dy_norm > 0 ? -this->s() : this->s() );

                int dist = std::min( std::max( std::max( abs(dx_norm), abs(dy_norm) ), abs( dy_norm - dx_norm ) ),
                                     std::max( std::max( abs(dx_wrap), abs(dy_wrap) ), abs( dy_wrap - dx_wrap ) ) );

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

