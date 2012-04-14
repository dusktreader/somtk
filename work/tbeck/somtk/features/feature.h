#pragma once

#include <QVector>
#include <QSharedPointer>

#include <cmath>
#include <fstream>
#include <ios>

namespace somtk {

class Feature;

typedef QSharedPointer<Feature> FeaturePtr;

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
    static double distance(
        Feature a, ///< The first feature to compare
        Feature b  ///< The second feature to compare
        );

    /// Calculates the Euclidean distance between this and another feature
    double distance(
        Feature other ///< The other feature to compare against
        ) const;

    /// Calculates the Euclidean distance between this and another feature
    double distance(
        FeaturePtr other ///< The other feature to compare against
        ) const;




    /// Adjusts the value of this Feature to make it more like an input feature
    static void adjust(
        Feature& a,        ///< The Feature to adjust
        Feature  b,        ///< The Feature by which to adjust
        double scaleFactor ///< The scaling factor by which to augment the adjustment
        );

    /// Adjusts the value of this Feature to make it more like an input feature
    void adjust(
        Feature other,     ///< The other Feature to compare against
        double scaleFactor ///< The scaling factor by which to augment the adjustment
        );

    /// Adjusts the value of this Feature to make it more like an input feature
    void adjust(
        FeaturePtr other,  ///< The other Feature to compare against
        double scaleFactor ///< The scaling factor by which to augment the adjustment
        );
};

} // namespace
