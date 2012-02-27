#pragma once

#include <limits>
#include <cfloat>


#include "normalizer.h"
#include "randmaster.h"

namespace hsom {

class MinMaxNormalizer : public Normalizer
{

private:

    /// The lower limit of normalized data
    double loLimit;

    /// The upper limit of normalized data
    double hiLimit;

    /// The minimum of encountered input data
    QVector<double> los;

    /// The maximum of encountered input data
    QVector<double> his;



protected:

    virtual void calculateNormalizer( QVector<FeaturePtr> features, QMap<QString, QVariant> nomalizerParameters );



public:

    MinMaxNormalizer();

    virtual void clear();
    virtual void normalize( Feature& feature );
    virtual void set( Feature& feature );
};

} // namespace hsom
