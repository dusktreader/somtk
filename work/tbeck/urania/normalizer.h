#pragma once

#include <QMap>
#include <QString>
#include <QVariant>
#include <QSharedPointer>

#include "feature.h"

namespace hsom {

class Normalizer
{

private:

    /// Indicates whether or not this normalizer has been computed
    bool _isCalculated;

    /// Provides random functionality if needed by derived instances
    static RandMaster randomizer;


protected:

    /// Describes the parameters used to tune the calculation of this normalizer
    QMap<QString, QVariant> _calculationParameters;



    // The Normalizer API

    virtual void calculateNormalizer(
            QVector<FeaturePtr> features,               ///< A sample of features for which to compute the normalization
            QMap<QString, QVariant> nomalizerParameters ///< The tuning parameters used to compute the normalization
            );



public:

    /// Constructs a Normalizer
    Normalizer();

    /// Computes the normalization statisticts to be used for future normalizations
    virtual void calculate(
            QVector<FeaturePtr> features,               ///< A sample of features for which to compute the normalization
            QMap<QString, QVariant> nomalizerParameters ///< The tuning parameters used to compute the normalization
            );



    // The Normalizer API

    /// Clears the normalizer
    virtual void clear() = 0;

    /// Normalizes a single feature
    virtual void normalize( Feature& feature ) = 0;

    /// Sets a features values based upon calculated normalization
    virtual void set( Feature& feature ) = 0;

};

typedef QSharedPointer<Normalizer> NormalizerPtr;

} // namespace hsom
