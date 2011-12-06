#pragma once

#include "opencv2/core/core.hpp"

#include "tools.hpp"

#include <QVector>
#include <QSharedPointer>

namespace hsom {

/// The feature class provides an abstract base class for SOM features
class Feature : public QVector<double>
{
    /// Generates random numbers for any features
    static RandMaster rng;

public:

    /// Constructs the base Feature class
    Feature();

    /// Constructs the Feature from a data vector
    Feature( const QVector<double>& other );

    /// Destructs the base Feature class
    virtual ~Feature();

    /// Calculates the Euclidean distance between this and another feature
    virtual double distance(
        const FeaturePtr other ///< The other feature to compare against
        ) const;

    /// Adjusts the value of this Feature to make it more like an input feature
    virtual void adjust(
        const FeaturePtr other, ///< The other Feature to compare against
        double scaleFactor      ///< The scaling factor by which to augment the adjustment
        );

    /// Initializes this Feature
    virtual void initialize();

};

typedef QSharedPointer<Feature> FeaturePtr;

} // namespace hsom
