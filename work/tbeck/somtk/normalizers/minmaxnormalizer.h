#pragma once

#include <limits>
#include <cfloat>


#include "normalizer.h"
#include "tools/randmaster.h"
#include "tools/localassert.h"
#include "tools/convert.hpp"

namespace somtk {

/// @see normalizer.h for full documentation of the Normalizer API
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

    /// @see normalizer.h for full documentation
    virtual void calculateNormalizer( QVector<DVectorPtr> vectors, QMap<QString, QVariant> nomalizerParameters );



public:

    MinMaxNormalizer();



    // The Normalizer API

    /// @see normalizer.h for full documentation
    virtual void normalize( DVectorPtr vectors );

    /// @see normalizer.h for full documentation
    virtual void setFeature( DVectorPtr vectors );
};

} // namespace
