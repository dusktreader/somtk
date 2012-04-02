#pragma once

#include <QSharedPointer>

#include "normalizer.h"

namespace hsom {

class NullNormalizer : public Normalizer
{

protected:

    virtual void calculateNormalizer(QVector<Feature>, QMap<QString, QVariant>);

public:

    NullNormalizer();

    virtual void normalize(Feature &);
    virtual void setFeature( Feature &feature );
    virtual void clear();
};

typedef QSharedPointer<NullNormalizer> NullNormalizerPtr;

} // namespace hsom
