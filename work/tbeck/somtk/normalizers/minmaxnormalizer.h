#pragma once

#include <limits>
#include <cfloat>


#include "normalizer.h"

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



public:

    MinMaxNormalizer();



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

} // namespace
