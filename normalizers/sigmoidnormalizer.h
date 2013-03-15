#pragma once

#include <QSharedPointer>

#include "normalizer.h"

namespace somtk {

/// @see normalizer.h for full documentation of the Normalizer API
class SigmoidNormalizer : public Normalizer
{

private:

    // Statistical metrics used for the normalization function

    /// The mean of the feature inputs
    QVector<double> normMean;

    /// The standard deviation of the feature inputs
    QVector<double> normStdv;

    /// The alpha tuning parameter??? of the feature inputs
    QVector<double> normAlpha;



    /// The epsilon value to use for the sigmoid squashing
    double epsilon;

    /// The number of standard deviations for which to calibrate the sigmoid curve
    double sigmaStep;



public:

    SigmoidNormalizer();



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

typedef QSharedPointer<SigmoidNormalizer> SigmoidNormalizerPtr;

} // namespace
