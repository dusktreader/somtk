#pragma once

#include <QVector>
#include <QSize>
#include <QPoint>
#include <QPointF>

#include "persist.h"

/// Defines the vertical distance between cells in the grid
#define HG_B 0.86602540378443837

namespace hsom {

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
        ASSERT_MSG( size.width() * size.height() == items.size(), "Size doesn't fit all items.  Size missmatch" );

        _size = size;
        _items = items;
        _distances = QVector<int>( _items.size() );
    }

    /// Destructs the HexGrid
    virtual ~HexGrid(){}

    /// Checks the supplied size to ensure that it is valid for the HexGrid
    void checkSize( QSize size )
    {
        ASSERT_MSG( size.width() > 0,       "Grid width must be greater than 0"                 );
        ASSERT_MSG( size.height() > 0,      "Grid height must be greater than 0"                );
        ASSERT_MSG( size.height() % 2 == 0, "Grids must have even height ( for edge wrapping )" );
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
                // Fetch the coordinate of the current index
                QPoint point = coords( i );

                // Calculate the distance ( on the hex grid ) to the origin point
                int dy = abs( origin.y() - point.y() );
                dy = std::min( dy, _size.height() - dy );

                int dx = abs( origin.x() - point.x() );
                dx = std::min( dx, _size().width() - dx );

                _distances[i] = dy - std::max( dx - dy / 2, 0 );

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
    unsigned l()
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
        ASSERT_MSG( point.x() >= 0 && point.x() < _size.w, "x coordinate must be in the range [ 0 , w )" );
        ASSERT_MSG( point.y() >= 0 && point.y() < _size.h, "y coordinate must be in the range [ 0 , h )" );
        return point.y() * _size.width() + point.x();
    }

    /// Fetches the coordinates given an index
    QPoint coords(
        int idx ///< The index in the grid for which to fetch the coordinates
        )
    {
        ASSERT_MSG( idx < l(), "The index must not be greater than the grid's area" );
        return QPoint( idx % _size.width(), idx / _size.width() );
    }

    /// Fetches the real coordinates of a given point in the grid ( should only be used for visualization )
    QPointF realCoords( QPoint point )
    {
        ASSERT_MSG( point.x() >= 0 && point.x() < _size.width(),  "x coordinate must be in the range [ 0 , w )" );
        ASSERT_MSG( point.y() >= 0 && point.y() < _size.height(), "y coordinate must be in the range [ 0 , h )" );
        return QPointF( point.x() + ( point.y() % 2 ) * 0.5, point.y() * HG_B );
    }

    /// @todo Re-do visualize function to draw a QImage
    /*
    cv::Mat visualize()
    {
        int globalMaxDist = 0;
        int localMaxDist = 0;
        #pragma omp parallel
        {
            #pragma omp for firstprivate( localMaxDist )
            for( int i=0; i<l(); i++ )
                localMaxDist = distances[i] > localMaxDist ? distances[i] : localMaxDist;
            #pragma omp critical
            {
                globalMaxDist = localMaxDist > globalMaxDist ? localMaxDist : globalMaxDist;
            }
        }
        int radius = 10;
        double buff = 2.5 * radius;
        PointPlus<double> pt( realCoords( PointPlus<int>( size().w - 1, size().h - 1 ) ) );
        SizePlus<int> vizSz( (int)( ( pt.x + 1 ) * buff ), (int)( ( pt.y + 1 ) * buff ) );
        cv::Mat viz( vizSz, CV_8UC1 );
        viz.setTo( 0 );
        for( int i=0; i<l(); i++ )
        {
            pt = realCoords( coords(i) );
            pt.x = pt.x * buff + 0.5 * buff;
            pt.y = pt.y * buff + 0.5 * buff;
            if( distances[i] == -1 )
                continue;
            cv::Scalar clr( ( 255 * ( globalMaxDist - distances[i] ) ) / globalMaxDist );
            cv::circle( viz, pt, radius, clr, CV_FILLED );
        }
        return viz;
    }*/

};

} // namespace hsom
