#pragma once

#include <QSharedPointer>

#include "normalizer.h"

namespace somtk {

/// @see normalizer.h for full documentation of the Normalizer API
class NullNormalizer : public Normalizer
{

protected:

    /// @see normalizer.h for full documentation
    virtual void calculateNormalizer( QVector<DVectorPtr>, QMap<QString, QVariant> );



public:

    NullNormalizer();



    // The Normalizer API
    virtual void clear();

    /// @see normalizer.h for full documentation
    virtual void normalize( DVectorPtr );

    /// @see normalizer.h for full documentation
    virtual void setFeature( DVectorPtr vector );
};

typedef QSharedPointer<NullNormalizer> NullNormalizerPtr;

} // namespace
