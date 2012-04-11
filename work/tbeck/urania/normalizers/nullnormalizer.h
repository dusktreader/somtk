#pragma once

#include <QSharedPointer>

#include "normalizer.h"

namespace hsom {

/// @see normalizer.h for full documentation of the Normalizer API
class NullNormalizer : public Normalizer
{

protected:

    /// @see normalizer.h for full documentation
    virtual void calculateNormalizer(QVector<Feature>, QMap<QString, QVariant>);



public:

    NullNormalizer();



    // The Normalizer API

    /// @see normalizer.h for full documentation
    virtual void normalize( Feature& );

    /// @see normalizer.h for full documentation
    virtual void setFeature( Feature &feature );
};

typedef QSharedPointer<NullNormalizer> NullNormalizerPtr;

} // namespace hsom
