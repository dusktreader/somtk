#pragma once

#include <QSize>
#include <QVector>
#include <QMap>
#include <QVariant>

#include <algorithm>
#include <cmath>
#include <cfloat>
#include <climits>
#include <iostream>

#include "features/feature.h"
#include "grids/grid.hpp"
#include "errors/somerror.h"
#include "normalizers/normalizer.h"

/// A constant describing the conversion factor between FWHM and sigma
#define FWHM_FACTOR 2.3548200450309493

namespace somtk {

typedef QSharedPointer< Grid<FeaturePtr> > FeatureGrid;

class SOM
{

private:

    /// The grid of features that represents the internal state of the SOMs
    FeatureGrid _grid;

    /// The normalizer for incoming features
    NormalizerPtr _normalizer;

    /// The number of training epochs to use
    int maxEpochs;

    /// The initial training weight
    double initialAlpha;

    /// The current training weight
    double currentAlpha;

    /// @todo  doc
    double alpha_tf;

    /// @todo  doc
    double alpha_Nf;

    /// @todo  doc
    double alpha_gamma;



    /// The initial radius of the training neighborhood
    int initialRadius;

    /// The current radius of the training neighborhood
    double currentRadius;

    /// @todo  doc
    double radius_tf;

    /// @todo  doc
    double radius_Nf;

    /// @todo  doc
    double radius_gamma;

    /// The current epoch in the training
    int currentEpoch;



    /// A vector of pre-calculated weights for updating a neighborhood
    QVector<double> weights;

    /// Precalculates the weights for updating a neighborhood
    void precalculateWeights();



public:

    /// Constructs an SOM with a specific grid
    SOM(
        FeatureGrid grid,        ///< The grid of features that the SOM will use internally
        NormalizerPtr normalizer ///< The normalizer used to adjust new features in the grid
        );

    /// Destructs this SOM
    virtual ~SOM();

    /// Initializes the SOM training process by resetting the epochs, alpha, radius, and the target feature type
    void initializeTraining(
        QMap<QString, QVariant> somParameters, ///< The parameters to be used for training this SOM
        QVector<FeaturePtr> features           ///< The features with which to train the SOM
        );

    void train(
        QMap<QString, QVariant> somParameters, ///< The tuning parameters to use for the training
        QVector<FeaturePtr> features,          ///< The features with which to train the SOM
        bool skipInit_debugOnly = false        ///< Only to be used for debugging
        );

    /** @brief  Advances the SOM to the next epoch
      * @return A boolean flag indicating training status.  A false value indicates that all epochs have been finished
      */
    bool nextEpoch();

    /// Trains the SOM with a single feature
    void update(
        FeaturePtr feature ///< The feature with which to update the SOM
        );

    /// Fetches the index of the cell that holds the closest feature in the SOM to an input feature
    int closestFeature(
        FeaturePtr feature ///< The input feature to compare against feature in the SOM
        );

    /// Fetches the underlying grid of features from the SOM
    FeatureGrid grid();

    /// Fetches the normalizer used by this SOM
    NormalizerPtr normalizer();

    /// Dumps the features from this SOM out to a regular vector
    QVector<Feature> dumpFeatures();
};

typedef QSharedPointer<SOM> SOMPtr;

} // namespace
