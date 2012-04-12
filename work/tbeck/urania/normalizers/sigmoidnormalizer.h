#pragma once

#include <QSharedPointer>

#include "normalizer.h"

/// A constant describing the conversion factor between FWHM and sigma
#define FWHM_FACTOR 2.3548200450309493

namespace hsom {

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

    /// The number of standard deviations fro which to calibrate the sigmoid curve
    int sigmaStep;



protected:

    /// @see normalizer.h for full documentation
    virtual void calculateNormalizer( QVector<Feature> features, QMap<QString, QVariant> normalizerParameters );



public:

    SigmoidNormalizer();

    // The Normalizer API

    /// @see normalizer.h for full documentation
    virtual void normalize( Feature& feature );

    /// @see normalizer.h for full documentation
    virtual void setFeature( Feature& feature );
};

typedef QSharedPointer<SigmoidNormalizer> SigmoidNormalizerPtr;

} // namespace hsom