#pragma once

#include <QVector>
#include <QSize>
#include <QPoint>
#include <QPointF>

#include "errors/somerror.h"


namespace hsom {

/// Defines the vertical distance between cells in the grid
#define HG_B 0.86602540378443837

/** The HexGrid class provides the basis for the spatial organization of the Self Organizing Map.  It provides a
  * hexagonal grid which supports neighborhood searches, edge wrapping, and other functionality.
  */
template <class T>
class HexGrid
{
protected:

    /// The dimensions of the hex grid
    QSize _size;

    /// A set of items for the slots of the grid
    QVector<T> _items;

    /// A set of distances for the slots of the grid in a given neighborhood
    QVector<int> _distances;



public:

    /// Constructs a hex grid with no size information
    HexGrid(){}

    /// Constructs the hex grid with a specified size
    HexGrid(
        QSize size ///< The size of the new grid
        )
    {
        setSize( size );
    }

    /// Constructs the hex grid with the specified size and fills it with the supplied values
    HexGrid(
            QSize size,      ///< The size of the new grid
            QVector<T> items ///< The items with which to populate the grid
            )
    {
        checkSize( size );
        SOMError::requireCondition( size.width() * size.height() == items.size(),
                                    "Size doesn't fit all items.  Size missmatch" );

        _size = size;
        _items = items;
        _distances = QVector<int>( _items.size() );
    }

    /// Destructs the HexGrid
    virtual ~HexGrid(){}

    /// Checks the supplied size to ensure that it is valid for the HexGrid
    void checkSize( QSize size )
    {
        SOMError::requireCondition( size.width() > 0,
                                    "Grid width must be greater than 0" );
        SOMError::requireCondition( size.height() > 0,
                                    "Grid height must be greater than 0" );
        SOMError::requireCondition( size.height() % 2 == 0,
                                    "Grids must have even height ( for edge wrapping )" );
    }

    /// Sets the size of the grid
    void setSize( QSize size )
    {
        checkSize( size );

        _size = size;
        _items  = QVector<T>( size.width() * size.height() );
        _distances = QVector<int>( _items.size() );
    }

    /// Clears the hex grid
    virtual void clear()
    {
        _items.clear();
        _distances.clear();
        _size.setWidth( 0 );
        _size.setHeight( 0 );
    }

    /// Sets all of the slots in the grid to the specified item
    void setTo( const T& item )
    {
        _items.fill( item );
    }

    /// Resets the size of the grid and re-initializes it
    void resetSize( const QSize& size )
    {
        clear();
        setSize( size );
    }

    /// Fetches the distance calculated for the given index
    int distance( int idx )
    {
        return _distances[idx];
    }

    /// Fetches the distance calculated for the given point
    int distance( QPoint& point )
    {
        return _distances[ index(point) ];
    }

    /// Fetches indices for all slots within a radius of the slot at x, y
    QVector<int> neighborhood(
        int r,              ///< The radius within which a cell must lie to be part of the neighborhood
        const QPoint& point ///< The origin cell for the neighborhood ( center cell )
        )
    {
        return neighborhood( r, index(point) );
    }

    /// Fetches indices for all slots within a radius of the specified slot
    QVector<int> neighborhood(
        int r,  ///< The radius within which a cell must lie to be a part of the neighborhood
        int idx ///< The origin index for the neighborhood ( center cell )
        )
    {
        // Fetch the coordinate of the origin index
        QPoint origin = coords( idx );

        // The neighbors vector will contain the indices of all cells in the neighborhood
        QVector<int> globalNeighbors;

        // The local neighbors vector will contain all cells within the neighborhood discovered by each omp thread
        QVector<int> localNeighbors;

        #pragma omp parallel
        {
            #pragma omp for private( localNeighbors )
            for( int i = 0; i < this->l(); i++ )
            {
                /* For each element in the loop, we will determine the direction and distance
                 * that we must step to get from the point represented by the index to the origin of
                 * the neighborhood
                 */

                // Fetch the coordinate of the current index
                QPoint point = coords( i );



                // Calculate the distance ( on the hex grid ) to the origin point
                int dist_y = abs( origin.y() - point.y() );

                // Calculate the distance if we were to move over the nearest y boudary
                int wrap_dist_y = _size.height() - dist_y;

                // If it is closer to wrap over the y boundary move that way
                if( wrap_dist_y < dist_y )
                    dist_y = wrap_dist_y;



                // Calculate the change in x to get from the point to the origin
                int delta_x = origin.x() - point.x();

                // Calculate the distance between the origin and the point
                int dist_x = abs( delta_x );

                // Indicates if the point lies on an even row
                bool even_row   = !( point.y() % 2 );

                // Indicates that the origin is to the right of the point
                bool move_right = delta_x >= 0;

                // Determine if the current movement gets an extra x for y movement
                int extra_x = move_right ^ even_row;

                // Calculate the distance if we were to move over the nearest x boundary
                int wrap_dist_x = _size.width() - dist_x;

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
                _distances[i] = dist_y + dist_x;

                // If the distance is within the radius, add this index to the local neighbors
                if( _distances[i] < r )
                    localNeighbors.append( i );
            }

            // Each thread adds it's local neighbor list to the global neighbor list
            #pragma omp critical
            {
                globalNeighbors += localNeighbors;
            }
        }
        return globalNeighbors;
    }

    /// Fetches the size of the grid
    QSize size()
    {
        return _size;
    }

    /// Fetches the linear size of the grid
    int l()
    {
        return _items.size();
    }

    /// Fetches the vector of items from this grid
    QVector<T> items()
    {
        return _items;
    }

    /// Fetches the item at a specific point in the grid
    T& operator[](
        const QPoint& point ///< The location in the grid from which to fetch an item
        )
    {
        return item( point );
    }

    /// Fetches the item at a specific index in the grid
    T& operator[](
        int idx ///< The index in the grid from which to fetch an item
        )
    {
        /// @todo Add a bounds checking here and custom exceptions would be nice
        return item( idx );
    }

    /// Fetches the item at a specific point in the grid
    T& item(
        const QPoint& point ///< The location in the grid from which to fetch an item
        )
    {
        return _items[ index( point ) ];
    }

    /// Fetches the item at a specific index in the grid
    T& item(
        int idx ///< The index in the grid from which to fetch an item
        )
    {
        return _items[ idx ];
    }

    /// Fetches the index for the slot at the specified coordinates
    int index(
        QPoint point ///< The point in the grid for which to fetch the index
        )
    {
        SOMError::requireCondition( point.x() >= 0 && point.x() < _size.width(),
                                    "x coordinate must be in the range [ 0 , w )" );
        SOMError::requireCondition( point.y() >= 0 && point.y() < _size.height(),
                                    "y coordinate must be in the range [ 0 , h )" );
        return point.y() * _size.width() + point.x();
    }

    /// Fetches the coordinates given an index
    QPoint coords(
        int idx ///< The index in the grid for which to fetch the coordinates
        )
    {
        SOMError::requireCondition( idx < l(),
                                    "The index must not be greater than the grid's area" );
        return QPoint( idx % _size.width(), idx / _size.width() );
    }

    /// Fetches the real coordinates of a given point in the grid ( should only be used for visualization )
    QPointF realCoords( QPoint point, double scale=1.0 )
    {
        SOMError::requireCondition( point.x() >= 0 && point.x() < _size.width(),
                                    "x coordinate must be in the range [ 0 , w )" );
        SOMError::requireCondition( point.y() >= 0 && point.y() < _size.height(),
                                    "y coordinate must be in the range [ 0 , h )" );
        return QPointF( ( point.x() + ( point.y() % 2 ) * 0.5 ) * scale,
                        ( point.y() * HG_B) * scale );
    }

};

} // namespace hsom
