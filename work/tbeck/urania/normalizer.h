#pragma once

#include <QMap>
#include <QString>
#include <QVariant>
#include <QSharedPointer>

#include "persist.h"
#include "feature.h"

namespace hsom {

class Normalizer : public PersistXML
{
    Q_OBJECT

private:

    /// Indicates whether or not this normalizer has been computed
    bool _isCalculated;



protected:

    /// Describes the parameters used to tune the calculation of this normalizer
    QMap<QString, QVariant> _calculationParameters;



    // The Normalizer API

    /// Interfaces with the PersistXML API to ensure correct loading order for a normalizer
    virtual void readNormalizerData( QDomElement& element );

    /// Interfaces with the PersistXML API to ensure correct saving order for a normalizer
    virtual void writeNormalizerData( QDomElement& element );

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
    virtual void normalize( FeaturePtr feature ) = 0;



    // The PersistXML API.

    virtual void readData( QDomElement& element );
    virtual void writeData( QDomElement& element );

};

typedef QSharedPointer<Normalizer> NormalizerPtr;

} // namespace hsom
