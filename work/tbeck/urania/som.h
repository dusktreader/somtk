#ifndef SOM_H
#define SOM_H

#include <QSize>
#include <QVector>
#include <QMap>
#include <QVariant>

#include <algorithm>

#include "feature.h"
#include "hexgrid.hpp"
#include "somerror.h"

/// A slope tuning parameter for an inverse exponential function
#define A 0.1

/// Adjustment parameter for the initial width of the neighborhood function
#define B 0.20

/// A constant describing the conversion factor between FWHM and sigma
#define FWHM_FACTOR 2.3548200450309493

namespace hsom {

class SOM
{

private:

    /// The hexagonal grid of features that represents the internal state of the SOMs
    HexGrid<FeaturePtr> grid;

    /// The number of training epochs to use
    int maxEpochs;

    /// The initial training weight
    int initialAlpha;

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

    /// A feature representing the type of features that this SOM will recognize
    FeaturePtr featureRep;

    /// Precalculates the weights for updating a neighborhood
    void precalculateWeights();



public:

    /// Creates an empty SOM
    SOM();

    /// Constructs an SOM with a specific size
    SOM(
        const QSize& size ///< The size of the grid underlying the SOm
         );

    /// Destructs this SOM
    virtual ~SOM();

    /// Fetches the size of this SO
    const QSize& size();

    /// Initializes the SOM training process by resetting the epochs, alpha, radius, and the target feature type
    void initializeTraining(
        QMap<QString, QVariant> somParameters,  ///< The parameters to be used for training this SOM
        FeaturePtr featureRep                   ///< Provides an example feature for the SOM to train with
        );

    /** @brief  Advances the SOM to the next epoch
      * @return A boolean flag indicating training status.  A false value indicates that all epochs have been finished
      */
    bool nextEpoch();

    /// Trains the SOM with a single feature
    void update(
        FeaturePtr feature ///< The feature with which to update the SOM
        );

    void train(
        QVector<FeaturePtr> features,         ///< The features with which to train the SOM
        QMap<QString, QVariant> somParameters ///< The tuning parameters to use for the training
        );

    /// Fetches the index of the cell that holds the closest feature in the SOM to an input feature
    int closestFeatureIndex(
        FeaturePtr feature ///< The input feature to compare against feature in the SOM
        );

    /** @brief  Fetches the closest feature in the SOM to an input feature
      * @return The coordinates of the cell that holds the feature that most closesly resembles the input feature
      */
    QPoint closestFeatureCoords(
        FeaturePtr feature
        );
};

typedef QSharedPointer<SOM> SOMPtr;

} // namespace hsom

#endif // SOM_H
