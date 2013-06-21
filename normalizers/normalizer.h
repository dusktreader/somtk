#pragma once

#include <QVector>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QSharedPointer>

#include "tools/randmaster/randmaster.h"
#include "features/feature.h"
#include "errors/somerror.h"

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



public:

    /// Constructs a Normalizer
    Normalizer();

    /// Destroys a Normalizer
    virtual ~Normalizer();

    /** @brief Computes the normalization statisticts to be used for future normalizations
      * @note  This function calls the virtual calculate function.  The calculateNormalizer function should
      *         never be called explicitly.
      */
    void calculateNormalizer(
            QVector<FeaturePtr> features ///< A sample of vectors for which to compute the normalization
            );

    /** @brief Normalizes a vector of features
      * @note  This function calls the virtual normalize function on each feature in the vector.
      */
    void normalizeAll(
            QVector<FeaturePtr> features ///< A sample of vectors for which to compute the normalization
            );

    /** @brief Public wrapper exposes the normalize() for derived classes.  Ensures that normalized flags in features are set
      * @note  The normalize method should be made protected for all derived normalizer classes
      */
    void normalizeFeature(
            FeaturePtr feature ///< The feature to normalize
            );

    /** @brief Public wrapper exposes the set() for derived classes.  Ensures that the normalizer is calculated before
      *        the normalizer can be used to set values in features.
      * @note  The set method should be made protected for all derived normalizer classes
      */
    void setFeature(
            FeaturePtr feature ///< The feature to set
            );



    // The Normalizer API

protected:

    /** @brief Computes the normalization statisticts to be used for future normalizations
      * @note  This function is called by the calculate() method in this base class.  It should never be called explicitly.
      *        This method should always be protected in any derived normalizer class
      *        This ensures that the method can be called by the calculateNormalizer method (which is public), but may not be directly
      *        call by any external code.
      */
    virtual void calculate(
            QVector<FeaturePtr> features ///< A sample of vectors for which to compute the normalization
            ) = 0;

    /** @brief Normalizes a single feature
      * @note  This function is called by the normalizeFeature() method in the base class.  It should never be called
      *        explicitly.  This method should always be protected in any derived normalizer class This ensures that the
      *        method can be called by the normalizeFeature method (which is public), but may not be directly
      *        called by any external code.  The normalized flag of the features is managed by the normalizeFeature
      *        method.  No normalization should be performed on a feture that is already normalized
      */
    virtual void normalize( FeaturePtr feature ) = 0;


    /** @brief Sets a features values based upon calculated normalization
      * @note  This function is called by the setFeature() method in the base class.  It should never be called
      *        explicitly.  This method should always be protected in any derived normalizer class This ensures that the
      *        method can be called by the setFeature method (which is public), but may not be directly
      *        called by any external code.  A feature cannot be set until the normalizer has been calculated
      */
    virtual void set( FeaturePtr feature ) = 0;

public:

    /// Initializes a Normalizer with a set of calculation parameters
    virtual void initialize(
            QMap<QString, QVariant> nomalizerParameters ///< The tuning parameters to be used in normalization
            ) = 0;

};

typedef QSharedPointer<Normalizer> NormalizerPtr;

} // namespace
