#pragma once

#include <QVector>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QSharedPointer>

#include "tools/randmaster.h"

namespace somtk {

typedef QVector<double> DVector;
typedef QSharedPointer<DVector> DVectorPtr;

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
            QVector<DVectorPtr> vectors ,               ///< A sample of vectors for which to compute the normalization
            QMap<QString, QVariant> nomalizerParameters ///< The tuning parameters used to compute the normalization
            ) = 0;



public:

    /// Constructs a Normalizer
    Normalizer();

    /** @brief Computes the normalization statisticts to be used for future normalizations
      * @note  This function calls the virtual calculateNormalizer function.  The calculateNormalizer function should
      *         never be called explicitly.
      */
    void calculate(
            QVector<DVectorPtr> vectors,                ///< A sample of vectors for which to compute the normalization
            QMap<QString, QVariant> nomalizerParameters ///< The tuning parameters used to compute the normalization
            );



    // The Normalizer API

    /// Clears the normalizer
    virtual void clear() = 0;

    /// Normalizes a single feature
    virtual void normalize( DVectorPtr vector ) = 0;

    /// Sets a features values based upon calculated normalization
    virtual void setFeature( DVectorPtr vector ) = 0;

};

typedef QSharedPointer<Normalizer> NormalizerPtr;

} // namespace
