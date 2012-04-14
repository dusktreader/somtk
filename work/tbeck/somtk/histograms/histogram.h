#pragma once

#include <QSharedPointer>

#include "opencv2/core/core.hpp"

#include "grids/grid.hpp"

namespace somtk {

class Histogram : public HexGrid<double>
{

protected:

    /// The grid that this Histogram operates over
    Grid<double>& _grid;

public:

    /// Constructs the Histogram with a supplied grid
    Histogram(
        Grid<double>& grid ///< The grid that this Histogram will use
        );

    /// Destructs the Histogram
    virtual ~Histogram();

    /// Resets this Histogram's bins to 0
    void reset();

    /// Increments a bin in the histogram
    void increment(
        QPoint point ///< The coordinates of the bin to increment
        );

    /// Increments a bin in the histogram
    void increment(
        int idx ///< The index of the bin to increment
        );

    /// Gets the value of a bin at the given index
    double bin(
        int idx ///< The index of the bin from wich to fetch the value
        );

    /// Gets the value of a bin at the given index
    double bin(
        QPoint point ///< The coordiates of the bin from which to fetch the value
        );

    /// Normalizes the histogram's bins
    /// @todo Add a parameter to choose normalization method
    void normalize();

    /// Creates a visualization of the normalized histogram
    /// @todo - Implement a visualization function that creates a QImage
    // cv::Mat visualize();

};

typedef QSharedPointer<Histogram> HistogramPtr;

} // namespace
