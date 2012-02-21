#pragma once

#include <QVector>

namespace hsom {

/// The feature class provides an abstract base class for SOM features
class Feature : public QVector<double>
{

public:

    /// Constructs an empty Feature
    Feature();

    /// Constructs a feature of a given size
    Feature( int size );

    /// Constructs the Feature from a data vector
    Feature( QVector<double> other );

    /// Calculates the Euclidean distance between this and another feature
    double distance(
        Feature other ///< The other feature to compare against
        ) const;

    /// Adjusts the value of this Feature to make it more like an input feature
    void adjust(
        Feature other,     ///< The other Feature to compare against
        double scaleFactor ///< The scaling factor by which to augment the adjustment
        );
};

} // namespace hsom
