#pragma once

#include <QVector>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QSharedPointer>

#include "tools/randmaster.h"
#include "features/feature.h"

namespace somtk {

/// @todo  Document Classes and files
class Normalizer
{

private:

    /// Indicates whether or not this normalizer has been computed
    bool _isCalculated;



protected:

    /// Describes the parameters used to tune the calculation of this normalizer
    QMap<QString, QVariant> _calculationParameters;

    /// Provides random functionality if needed by derived instances
    RandMaster randomizer;



    // The Normalizer API

    /** @brief Computes the normalization statisticts to be used for future normalizations
      * @note  This function is called by calculate() in this base class.  It should never be called explicitly.
      */
    virtual void calculateNormalizer(
            QVector<FeaturePtr> features ///< A sample of vectors for which to compute the normalization
            ) = 0;



public:

    /// Constructs a Normalizer
    Normalizer();

    /** @brief Computes the normalization statisticts to be used for future normalizations
      * @note  This function calls the virtual calculateNormalizer function.  The calculateNormalizer function should
      *         never be called explicitly.
      */
    void calculate(
            QVector<FeaturePtr> features,               ///< A sample of vectors for which to compute the normalization
            QMap<QString, QVariant> nomalizerParameters ///< The tuning parameters used to compute the normalization
            );

    /** @brief Normalizes a vector of features
      * @note  This function calls the virtual normalize function on each feature in the vector.
      */
    void normalize(
            QVector<FeaturePtr> features ///< A sample of vectors for which to compute the normalization
            );



    // The Normalizer API

    /// Normalizes a single feature
    virtual void normalize( FeaturePtr vector ) = 0;

    /// Sets a features values based upon calculated normalization
    virtual void setFeature( FeaturePtr vector ) = 0;

};

typedef QSharedPointer<Normalizer> NormalizerPtr;

} // namespace
