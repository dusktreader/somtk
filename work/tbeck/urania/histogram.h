#pragma once

#include <QSharedPointer>

#include "opencv2/core/core.hpp"

#include "hexgrid.hpp"
#include "tools.hpp"

namespace hsom {

class Histogram : public HexGrid
{

public:

    /// Constructs a Histogram with default values
    Histogram();

    /// Constructs the Histogram with specified size
    Histogram(
        const QSize& size ///< The desired size of the histogram
        );

    /// Destructs the Histogram
    virtual ~Histogram();

    /// Resets this Histogram's bins to 0
    void reset();

    /// Increments a bin in the histogram
    void increment(
        const QPoint& point ///< The coordinates of the bin to increment
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
        const QPoint& point ///< The coordiates of the bin from which to fetch the value
        );

    /// Normalizes the histogram's bins
    /// @todo Add a parameter to choose normalization method
    void normalize();

    /// Creates a visualization of the normalized histogram
    /// @todo - Implement a visualization function that creates a QImage
    // cv::Mat visualize();

};

typedef QSharedPointer<Histogram> HistogramPtr;

} // namespace hsom
