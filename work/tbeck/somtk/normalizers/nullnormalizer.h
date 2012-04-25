#pragma once

#include <QSharedPointer>

#include "normalizer.h"

namespace somtk {

/// @see normalizer.h for full documentation of the Normalizer API
class NullNormalizer : public Normalizer
{

protected:

    /// @see normalizer.h for full documentation
    virtual void calculateNormalizer( QVector<FeaturePtr>, QMap<QString, QVariant> );



public:

    NullNormalizer();



    // The Normalizer API

    /// @see normalizer.h for full documentation
    virtual void normalize( FeaturePtr );

    /// @see normalizer.h for full documentation
    virtual void setFeature( FeaturePtr vector );
};

typedef QSharedPointer<NullNormalizer> NullNormalizerPtr;

} // namespace
