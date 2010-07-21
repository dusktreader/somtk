#pragma once

#include "cv.h"
#include "cxcore.h"

#include "hexgrid.hpp"
#include "tools.hpp"

class SOMHistogram
{
private:

    /** The grid that contains the histogram's bins */
    HexGrid<double> grid;

public:

    /** Constructs a SOMHistogram with default values */
    SOMHistogram();

    /** Constructs the SOMHistogram with specified size
      * @param  w - The desired size of the histogram
      */
    SOMHistogram( const SizePlus<int>& sz );

    virtual ~SOMHistogram();

    /** Increments a slot in the histogram
      * @param  x - The coordinates of the slot
      */
    void increment( const PointPlus<int>& pt );

    /** Increments a slot in the histogram
      * @param  idx - The slot index
      */
    void increment( int idx );

    /** Creates a visualization of the normalized histogram */
    void vizHistogram();


    /** Gets the value of a bin at the given index
      * @param  idx - The index to fetch the value from
      */
    double bin( int idx );

    /** Normalizes the histogram to itself */
    void normalize();

};

#endif                                                                                                                  // SOMHISTOGRAM_H

