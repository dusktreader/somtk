#pragma once

#include <iostream>

#include <QVector>
#include <QColor>
#include <QImage>
#include <QPainter>

#include "errors/somerror.h"

namespace somtk {

template <class T>
class Grid
{
private:

    /// A set of items for the slots of the grid
    QVector<T> _items;



protected:

    /// A vector describing the size of the grid
    QVector<int> _size;

    /// Sets the size of the grid
    void setCapacity( int capacity )
    {
        SOMError::requireCondition( capacity > 0,
                                    "Capacity must be greater than 0");
        _items  = QVector<T>( capacity );
    }



public:

    /// Constructs a hex grid with no size information
    Grid(){}

    /// Destructs the Grid
    virtual ~Grid(){}

    /// Initializes the hex grid with the specified size
    void init(
            QVector<int> size ///< The size of grid to create
            )
    {
        setSize( size );
    }

    /// Initializes the hex grid with the specified size and fills it with the supplied values
    void init(
            QVector<int> size, ///< The size of grid to create
            QVector<T> items   ///< The items with which to populate the grid
            )
    {
        _items.clear();
        setSize( size );
        setItems( items );
    }

    /// Sets the items to the supplied items
    void setItems(
            QVector<T> items /// The items with which to populate the grid
            ) {
        SOMError::requireCondition( items.size() == this->items().size(),
                                    "The number of items exceeds the capacity of this Grid" );
        this->_items = items;
    }

    /// Clears the grid
    void clear()
    {
        _items.clear();
    }

    /// Sets all of the slots in the grid to the specified item
    void setTo( const T& item )
    {
        _items.fill( item );
    }

    /// Fetches indices for all slots within a radius of the slot at the given point
    QVector<int> neighborhood(
        int r,             ///< The radius within which a cell must lie to be part of the neighborhood
        QVector<int> point ///< The origin cell for the neighborhood ( center cell )
        )
    {
        return neighborhood( r, index(point) );
    }

    /// Sets the size of the hex grid
    virtual void setSize( QVector<int> size )
    {
        this->setCapacity( this->capacityFromSize( size ) );
        this->_size = size;
    }

    /// Fetches the size of the grid
    QVector<int> size()
    {
        return this->_size;
    }

    /// Fetches the linear capacity of the grid
    int capacity()
    {
        return this->_items.size();
    }

    /// Fetches the vector of items from this grid
    QVector<T> items()
    {
        return this->_items;
    }

    /// Fetches the item at a specific index in the grid
    T& operator[](
        int idx ///< The index in the grid from which to fetch an item
        )
    {
        /// @todo Add a bounds checking here and custom exceptions would be nice
        return item( idx );
    }

    /// Fetches the item at a specific index in the grid
    virtual T& item(
        int idx ///< The index in the grid from which to fetch an item
        )
    {
        return this->_items[ idx ];
    }

    /// Ensures that a given index is valid for this grid
    void checkIndex(
            int idx ///< The index to check for validity
            )
    {
        SOMError::requireCondition( idx >= 0,
                                    QString( "Negative indices are invalid: index was %1" )
                                    .arg( idx ) );

        SOMError::requireCondition( idx < capacity(),
                                    QString( "Index exceeds capacity: index was %1, capacity was %2" )
                                    .arg( idx ).arg( capacity() ) );
    }

    /// Fetches indices for all slots within a radius of the specified slot
    QVector< QPair<int, int> > neighborhood(
        int r,  ///< The radius within which a cell must lie to be a part of the neighborhood
        int idx ///< The origin index for the neighborhood ( center cell )
        )
    {
        // The neighbors vector will contain the indices of all cells in the neighborhood
        QVector< QPair<int, int> > globalNeighbors;

        // The local neighbors vector will contain all cells within the neighborhood discovered by each omp thread
        QVector< QPair<int, int> > localNeighbors;

        #pragma omp parallel shared( globalNeighbors) private( localNeighbors )
        {
            #pragma omp for
            for( int i = 0; i < this->capacity(); i++ )
            {
                int distance = this->distance( idx, i );

                if( distance < r )
                    localNeighbors.append( QPair<int, int>( i, distance ) );
            }

            // Each thread adds it's local neighbor list to the global neighbor list
            #pragma omp critical
            {
                globalNeighbors += localNeighbors;
            }
        }
        return globalNeighbors;
    }

    /// Visualizes a hex grid given a function pointer to visualize its contents
    QImage visualize( double cellRadius, QColor (*render)( T ) )
    {
        QVector<double> bounds = realBounds();
        double W = bounds[0];
        double H = bounds[1];

        QSize imageSize = QSize( W * cellRadius * 2,
                                 H * cellRadius * 2 );
        QImage viz = QImage( imageSize,
                             QImage::Format_RGB888 );

        viz.fill( Qt::black );
        QPainter vizPainter;
        vizPainter.begin( &viz );

        for( int idx=0; idx < this->capacity(); idx++ )
        {
            QVector<double> realCoords = this->realCoords( idx );
            double x = realCoords[0] * 2 * cellRadius + cellRadius;
            double y = realCoords[1] * 2 * cellRadius + cellRadius;
            QColor color = render( this->item( idx ) );
            vizPainter.setPen( QPen( QColor( 255, 255, 255 ) ) );
            vizPainter.setBrush( QBrush( color ) );
            vizPainter.drawEllipse( QPointF( x, y ), cellRadius, cellRadius );
        }
        vizPainter.end();
        return viz;
    }



    // Grid API

    /// Creates a duplicate of this grid
    virtual QSharedPointer< Grid<T> > clone() = 0;

    /// Gets the capacity of a grid based on the size of the grid
    virtual int capacityFromSize( QVector<int> size ) = 0;

    /// Checks the supplied size to ensure that it is valid for the Grid
    virtual void checkSize( QVector<int> size ) = 0;

    /// Checks a given point to ensure it is a valid reference to an element in the grid
    virtual void checkCoords(
        QVector<int> coords ///< The point in the grid for which to check for validity
        ) = 0;

    /// Fetches the index for the slot at the specified coordinates
    virtual int index(
        QVector<int> coords ///< The point in the grid for which to fetch the index
        ) = 0;

    /// Fetches the coordinates given an index
    virtual QVector<int> coords(
        int idx ///< The index in the grid for which to fetch the coordinates
        ) = 0;

    /// Fetches the length of the major diagonal running through the grid
    virtual int diagonal() = 0;

    /// Fetches the distance between two cells in the grid
    virtual int distance( int idx0, int idx1 ) = 0;

    /// Fetches the extents of the real coordinate mapping space ( should only be used for visualization )
    virtual QVector<double> realBounds() = 0;

    /// Fetches the real coordinates of a given index in the grid ( should only be used for visualization )
    virtual QVector<double> realCoords( int idx ) = 0;
};

} // namespace
