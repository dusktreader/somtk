#pragma once

#include <QSharedPointer>

#include "grids/grid.hpp"

namespace somtk {

typedef QSharedPointer< Grid<double> > HistogramGrid;

class Histogram
{

protected:

    /// The grid that this Histogram operates over
    HistogramGrid _grid;

    /// Helper funtion to be used by the visualize function.  Renders the color for a ghistogram grid item
    static QColor render(
            double item ///< The value of the item to be rendered
            );



public:

    /// Constructs the Histogram with a supplied grid
    Histogram(
        HistogramGrid grid ///< The grid that this Histogram will use
        );

    /// Resets this Histogram's bins to 0
    void reset();

    /// Increments a bin in the histogram
    void increment(
        int idx ///< The index of the bin to increment
        );

    /// Gets the value of a bin at the given index
    double bin(
        int idx ///< The index of the bin from wich to fetch the value
        );

    /// Normalizes the histogram's bins
    /// @todo Add a parameter to choose normalization method
    void normalize();

    /// Creates a visualization of the normalized histogram
    QImage visualize();

};

typedef QSharedPointer<Histogram> HistogramPtr;

} // namespace
