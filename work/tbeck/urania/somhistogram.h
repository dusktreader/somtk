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
    
    /** Rests this SOMHistogram's bins to 0 */
    void reset();

    /** Increments a slot in the histogram
      * @param  x - The coordinates of the slot
      */
    void increment( const PointPlus<int>& pt );

    /** Increments a slot in the histogram
      * @param  idx - The slot index
      */
    void increment( int idx );

    /** Creates a visualization of the normalized histogram */
    cv::Mat vizHistogram();

    
    /** Gets the value of a bin at the given index
    * @param  idx - The index of the bin from wich to fetch the value
    */
    double bin( int idx );
    
    /** Gets the value of a bin at the given index
    * @param  pt - The coordiates of the bin from which to fetch the value
    */
    double bin( const PointPlus<int>& pt );

    /** Normalizes the histogram to itself */
    void normalize();

};

#endif                                                                                                                  // SOMHISTOGRAM_H

