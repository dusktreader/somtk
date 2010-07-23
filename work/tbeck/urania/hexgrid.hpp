#pragma once

#include <deque>

#include "tools.hpp"
#include "cvtypesplus.hpp"

#include "feature.h"

/** Enumration for marks in the HexGrid */
enum Mark { HG_DEFAULT_MARK, HG_WINNING_MARK, HG_VISITED_MARK, HG_ORIGIN_MARK, HG_NEIGHBOR_MARK, HG_EXCLUDE_MARK };

/** The default width of a HexGrid */
#define HG_DEFAULT_W      64

/** The default height of a HexGrid */
#define HG_DEFAULT_H      64

/** Default value for cells */
#define HG_DEFAULT_VALUE  0.0

/** Defines the vertical distance between cells in the grid */
#define HG_B              0.86602540378443837

/** The HexGrid class provides the basis for the spatial organization of the
  * Self Organizing Map.  It provides a hexagonal grid which supports
  * neighborhood searches, edge wrapping, and other functionality.
  */
template <class T>
class HexGrid{
protected:

    /** The alias for this class */
    static const std::string alias;

    /** The width of the hex grid */
    SizePlus<int> _sz;

    /** A set of items for the slots of the grid */
    std::vector<T> items;

    /** A set of real values for the slots of the grid */
    std::vector<double> values;

    /** A set of markers for the slots of the grid */
    std::vector<Mark> marks;

    /** Clears the hex grid */
    virtual void clear()
    {
        items.clear();
        _sz = SizePlus<int>();
    }

    /** Fetches the neighbors for the slot at the specified coordinates
      * @param  pt - The point coordinate of the slot
      * @return A vector of indices for the immediate neighbors ( <=6 indices )
      * @note   Will have fewer than 6 if the slot is on the border and
      *         wrapping is not available
      */
    std::vector<int> neighborIndices( const PointPlus<int>& pt )
    {
        ASSERT_MSG( pt.x >= 0 && pt.x < _sz.w, "x coordinate must be in the range [ 0 , w )" );
        ASSERT_MSG( pt.y >= 0 && pt.y < _sz.h, "y coordinate must be in the range [ 0 , h )" );
        std::vector<int> indices;                                                                                       // Will contain the indices of all neighbor cells
        int o = ( pt.y + 1 ) % 2;                                                                                       // An offset for rows.  If this row is odd ( shorter by 1 ) the offset is 1.  Alleviates problem with staggering;
        indices.push_back( index( PointPlus<int>( mod(   pt.x-1, _sz.w ),                 pt.y ) ) );                   // Neighbor to the left.  Notice that we use correct modular arithmethic to wrap around edges.  The % operator will not perform correctly here
        indices.push_back( index( PointPlus<int>( mod(   pt.x+1, _sz.w ),                 pt.y ) ) );                   // Neighbor to the right
        indices.push_back( index( PointPlus<int>( mod(   pt.x-o, _sz.w ), mod( pt.y-1, _sz.h ) ) ) );                   // Neighbor to the up-left
        indices.push_back( index( PointPlus<int>( mod( pt.x+1-o, _sz.w ), mod( pt.y-1, _sz.h ) ) ) );                   // Neighbor to the up-right
        indices.push_back( index( PointPlus<int>( mod(   pt.x-o, _sz.w ), mod( pt.y+1, _sz.h ) ) ) );                   // Neighbor to the down-left
        indices.push_back( index( PointPlus<int>( mod( pt.x+1-o, _sz.w ), mod( pt.y+1, _sz.h ) ) ) );                   // Neighbor to the down-right
        return indices;
    }

    /** Fetches the neighbors for the specified slot
      * @param  idx - The index of the slot
      * @return A vector of indices for the immediate neighbors ( <=6 indices )
      * @note   Will have fewer than 6 if the slot is on the border and
      *         wrapping is not available
      */
    std::vector<int> neighborIndices( int idx )
    {
        return neighborIndices( coords( idx ) );
    }

    /** Fetches indices for all slots within a radius of the slot at x, y
      * @param  r  - The radius within which to search
      * @param  pt - The point coordinate of the slot
      * @return A vector of indices of neighborhood slots
      */
    std::vector<int> neighborhood( double r, const PointPlus<double>& pt )
    {
        return neighborhood( r, index(pt) );
    }

    /** Fetches indices for all slots within a radius of the specified slot
      * @param  r     - The radius within which to search
      * @param  idx  - The index of the slot
      * @return A vector of indices of neighborhood slots
      */
    std::vector<int> neighborhood( double r, int idx )
    {
        resetMarks();                                                                                                   // The marks vector will be used to keep track of which cells have been visited
        resetValues();                                                                                                  // The values vector will be used to store the distance of a cell from the neighborhood's origin cell
        std::vector<int> results;                                                                                       // The results vector will contain the indices of all cells in the neighborhood
        std::deque<int> pending;                                                                                        // The pending vector will hold potential neighbors awaiting examination
        std::vector<int> neighbors;                                                                                     // The neighbors vector will temporarily hold indices of immediate neighbors
        marks[idx] = HG_ORIGIN_MARK;                                                                                    // All visited cells should be marked with something besides their default mark.  The origin cell is the only cell marked as the winner.
        values[idx] = 0.0;                                                                                              // Obviously the distance between the origin and itself is 0.0
        pending.push_back( idx );                                                                                       // Put the origin cell into the queue to begin the breadth first traversal
        double dist;                                                                                                    // Temporary distance variable holds the current distance
        while( !pending.empty() )                                                                                       // Process idices in the queue until the queue is empty ( meaning the traversal is done )
        {
            idx = pending.front();                                                                                      // Get the index at the front of the queue
            pending.pop_front();
            results.push_back( idx );                                                                                   // Add the popped index to the results
            dist = values[idx];                                                                                         // Record the current distance
            if( r - dist < EPSILON )                                                                                    // If the current distance is greater than or equal to the radius limit, do not examine the neighbors of the current cell.
                continue;
            neighbors = neighborIndices( idx );                                                                         // Otherwise, get all the neighbors of the current cell
            #pragma omp parallel private( idx )
            {                                                                                                           // +omp
                #pragma omp for
                for( int i=0; i<(int)neighbors.size(); i++ )
                {
                    idx = neighbors[i];
                    if( marks[idx] != HG_DEFAULT_MARK )                                                                 // If the current neighbor has already been marked, it has already been examined.  Pass it up
                        continue;
                    values[idx] = dist + 1.0;                                                                           // Otherwise record its distance.  For simplicity, distances are simply a count of the number of steps away from the origin cell.
                    marks[idx] = HG_NEIGHBOR_MARK;                                                                      // Mark the cell as being a part of the neighborhood.  This ensures that each neighbor cell is examined only once.
                    #pragma omp critical
                    {
                        pending.push_back( idx );                                                                       // Add the cell to the queue
                    }
                }
            }                                                                                                           // -omp
        }
        return results;
    }

public:

    /** Constructs the hex grid with default values */
    HexGrid(){}

    /** Constructs the hex grid with a specified size
      * @param  sz - The desired size of the hex grid
      */
    HexGrid( const SizePlus<int>& sz )
    {
        setSize( sz );
    }

    virtual ~HexGrid(){}

    /** Sets the size of the grid
      * @param  sz - The desired size of the hex grid
      */
    void setSize( const SizePlus<int>& sz )
    {
        ASSERT_MSG( sz.w > 0,      "Grid width must be greater than 0"             );
        ASSERT_MSG( sz.h > 0,      "Grid height must be greater than 0"            );
        ASSERT_MSG( sz.h % 2 == 0, "Grids must have even height for edge wrapping" );

        _sz = sz;
        items  = std::vector<T>(      l() );
        values = std::vector<double>( l() );
        marks  = std::vector<Mark>(   l() );
    }

    /** Resets the size of the grid and re-initializes it
      * @param  sz - The desired size of the hex grid
      */
    void resetSize( const SizePlus<int>& sz )
    {
        clear();
        setSize( sz );
    }

    /** Fetches the size of the grid */
    SizePlus<int> size()
    {
        return _sz;
    }

    /** Fetches the linear size of the grid */
    int l()
    {
        return _sz.area();
    }

    /** Resets the slots of the hex grid */
    void reset()
    {
        for( unsigned int i=0; i<l(); i++ )
        {
            marks[i] = HG_DEFAULT_MARK;
            values[i] = HG_DEFAULT_VALUE;
        }
    }

    /** Resets the markers for the slots of the grid */
    void resetMarks()
    {
        for( int i=0; i<l(); i++ )
            marks[i] = HG_DEFAULT_MARK;
    }

    /** Resets the values for the slots of the grid */
    void resetValues()
    {
        for( int i=0; i<l(); i++ )
            values[i] = HG_DEFAULT_VALUE;
    }

    T& operator[]( const PointPlus<int>& pt )
    {
        return item( pt );
    }

//    T& operator[]( int x, int y )
//    {
//        return item( PointPlus<int>( x, y ) );
//    }

    T& operator[]( int idx )
    {
        return item( idx );
    }

    /** Fetches the item at the given index
      * @param  idx - The index of the desired slot
      * @return A reference to the desired item
      */
    T& item( int idx )
    {
        return items[ idx ];
    }
    /** Fetches the item at the given coordinates
      * @param  pt - The coordinates of the desired slot
      * @return A reference to the desired item
      */
    T& item( const PointPlus<int>& pt )
    {
        return items[ index(pt) ];
    }

    /** Fetches the index for the slot at the specified coordinates
      * @param  pt - The point coordinate
      * @return The vector index of the corresponding slot
      */
    int index( const PointPlus<int>& pt )
    {
        ASSERT_MSG( pt.x >= 0 && pt.x < _sz.w, "x coordinate must be in the range [ 0 , w )" );
        ASSERT_MSG( pt.y >= 0 && pt.y < _sz.h, "y coordinate must be in the range [ 0 , h )" );
        return pt.y * _sz.w + pt.x;
    }

    /** Fetches the coordinates given an index
      * @param  index - The index of the slot for which to fetch coords
      * @return The point coordinate for the slot
      */
    PointPlus<int> coords( int idx )
    {
        ASSERT_MSG( idx < _sz.area(), "The index must not be greater than the grid's area" );
        return PointPlus<int>( idx % _sz.w, idx / _sz.w );
    }

    PointPlus<double> realCoords( const PointPlus<int>& pt )
    {
        ASSERT_MSG( pt.x >= 0 && pt.x < _sz.w, "x coordinate must be in the range [ 0 , w )" );
        ASSERT_MSG( pt.y >= 0 && pt.y < _sz.h, "y coordinate must be in the range [ 0 , h )" );
        return PointPlus<double>( pt.x + ( pt.y % 2 ) * 0.5, pt.y * HG_B );
    }

};
