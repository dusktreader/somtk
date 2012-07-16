#pragma once

#include <QSharedPointer>

#include "normalizer.h"

namespace somtk {

/// @see normalizer.h for full documentation of the Normalizer API
class NullNormalizer : public Normalizer
{

public:

    NullNormalizer();



// The Normalizer API

protected:

    /// @see normalizer.h for full documentation
    virtual void calculate( QVector<FeaturePtr> features );

    /// @see normalizer.h for full documentation
    virtual void normalize( FeaturePtr vectors );

    /// @see normalizer.h for full documentation
    virtual void set( FeaturePtr vectors );

public:

    /// @see normalizer.h for full documentation
    virtual void initialize( QMap<QString, QVariant> nomalizerParameters );
};

typedef QSharedPointer<NullNormalizer> NullNormalizerPtr;

} // namespace
