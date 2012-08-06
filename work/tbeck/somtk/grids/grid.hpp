#pragma once

#include <iostream>

#include <QVector>
#include <QQueue>
#include <QColor>
#include <QImage>
#include <QPainter>
#include <QMutex>

#include "errors/somerror.h"

namespace somtk {

template <class T>
class Grid
{
public:

    enum NeighborhoodMethod
    {
        GLOBAL,
        LOCAL,
        CACHED
    };

private:

    /// A set of items for the slots of the grid
    QVector<T> _items;

    /// A cache of computed neighborhoods for each item
    QVector< QVector< QVector<int> > > _neighborhoodCache;

    /// Describes the method used to compute the neighborhood for a given index
    NeighborhoodMethod _method;



protected:

    /// A vector describing the size of the grid
    QVector<int> _size;

    /// Sets the size of the grid
    void setCapacity( int capacity )
    {
        SOMError::requireCondition( capacity > 0, "Capacity must be greater than 0" );
        _items  = QVector<T>( capacity );
    }



public:

    /// Constructs a grid with no size information
    Grid(){}

    /// Destructs the Grid
    virtual ~Grid(){}



    /// Initializes the hex grid with the specified size
    void init(
            QVector<int> size ///< The size of grid to create
            )
    {
        setSize( size );
        _method = CACHED;
        initCachedNeighborhood();
    }

    /// Initializes the hex grid with a specified size and sets the neighborhood method
    void init(
            QVector<int> size,        ///< The size of the grid to create
            NeighborhoodMethod method ///< The neighborood method to use
            )
    {
        setSize( size );
        _method = method;
        if( _method == CACHED )
            initCachedNeighborhood();
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

    /// Sets the size of the hex grid
    virtual void setSize( QVector<int> size )
    {
        int capacity = capacityFromSize( size );
        this->setCapacity( capacity );
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

    /// Fetches indices for all slots within a radius of the slot at the given point
    QVector< QPair<int, int> > neighborhood(
        int r,             ///< The radius within which a cell must lie to be part of the neighborhood
        QVector<int> point ///< The origin cell for the neighborhood ( center cell )
        )
    {
        return neighborhood( r, index(point) );
    }

    /// Fetches indices for all slots within a radius of the specified slot
    QVector< QPair<int, int> > neighborhood(
        int r,  ///< The radius within which a cell must lie to be a part of the neighborhood
        int idx ///< The origin index for the neighborhood ( center cell )
        )
    {
        QVector< QPair<int, int> > neighborhood;
        switch( _method )
        {
            case GLOBAL : neighborhood = globalNeighborhood( r, idx ); break;
            case LOCAL  : neighborhood = localNeighborhood( r, idx ); break;
            case CACHED : neighborhood = cachedNeighborhood( r, idx ); break;
            default: SOMError::requireCondition( false, "Invalid method" );
        }
        return neighborhood;
    }

    /// Calculates the neighborhood cache
    void initCachedNeighborhood()
    {
        int maxDistance = diagonal();
        int dist;
        int n = capacity();

        _neighborhoodCache = QVector< QVector< QVector<int> > >( n, QVector< QVector<int> >( maxDistance + 1 ) );

        #pragma omp parallel for private( dist )
        for( int index = 0; index < n; index++ )
        {
            for( int neighbor = 0; neighbor < n; neighbor++ )
            {
                dist = distance( index, neighbor );
                SOMError::requireCondition(
                            dist <= maxDistance,
                            "Found a distance > diagonal.  SHOULD NOT HAPPEN! Check the diagonal() method"
                            );
                _neighborhoodCache[index][dist] << neighbor;
            }
        }
    }

    /// Fetches indices for all slots within a radius of the specified slot
    QVector< QPair<int, int> > cachedNeighborhood(
        int r,  ///< The radius within which a cell must lie to be a part of the neighborhood
        int idx ///< The origin index for the neighborhood ( center cell )
        )
    {
        QVector< QVector<int> > cache = _neighborhoodCache[idx];
        QVector< QPair<int, int> > neighborhood;
        for( int dist = 0; dist <= r; dist++ )
            foreach( int neighbor, cache[dist] )
                neighborhood << QPair<int, int>( neighbor, dist );
        return neighborhood;
    }

    /// Fetches indices for all slots within a radius of the specified slot
    QVector< QPair<int, int> > localNeighborhood(
        int r,  ///< The radius within which a cell must lie to be a part of the neighborhood
        int idx ///< The origin index for the neighborhood ( center cell )
        )
    {
        QVector<int> distances( capacity(), INT_MAX );
        QVector<int> unprocessed;
        QVector< QPair<int, int> > neighborhood;
        distances[idx] = 0;
        unprocessed << idx;

        QMutex locks[capacity()];

        while( !unprocessed.isEmpty() )
        {
            //#pragma omp parallel
            {
                QVector<int> myUnprocessed;
                QVector< QPair<int, int> > myNeighborhood;

                //#pragma omp for
                for( int i = 0; i < unprocessed.size(); i++ )
                {
                    int myIndex = unprocessed[i];
                    QVector<int> myNeighbors = neighbors( myIndex );
                    int myDistance = distances[myIndex];

                    foreach( int neighborIndex, myNeighbors )
                    {
                        bool addNeighbor = false;
                        locks[neighborIndex].lock();
                        int neighborDistance = myDistance + 1;
                        if( neighborDistance <= r && distances[neighborIndex] > neighborDistance )
                        {
                            distances[neighborIndex] = neighborDistance;
                            addNeighbor = true;
                        }
                        locks[neighborIndex].unlock();
                        if( addNeighbor )
                            myUnprocessed << neighborIndex;
                    }

                    myNeighborhood << QPair< int, int >( myIndex, myDistance );
                }
\
                //#pragma omp single
                {
                    unprocessed.clear();
                }

                //#pragma omp critical
                {
                    unprocessed << myUnprocessed;
                }

                //#pragma omp critical
                {
                    neighborhood << myNeighborhood;
                }
            }
        }

        return neighborhood;
    }
\
    /// Fetches indices for all slots within a radius of the specified slot
    /// @note This method will be faster than the normal neighborhood method for small maps
    QVector< QPair<int, int> > globalNeighborhood(
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

    /// Fetches the immediate neighbors of a given inde3x
    virtual QVector<int> neighbors( int idx ) = 0;
};

} // namespace
