#pragma once

#include <QSharedPointer>

#include "normalizer.h"

namespace hsom {

class NullNormalizer : public Normalizer
{

protected:

    virtual void calculateNormalizer( QVector<Feature> features, QMap<QString, QVariant> nomalizerParameters );

public:

    NullNormalizer();

    virtual void normalize( Feature &feature );
    virtual void setFeature( Feature &feature );
    virtual void clear();
};

typedef QSharedPointer<NullNormalizer> NullNormalizerPtr;

} // namespace hsom
