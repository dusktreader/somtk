#pragma once

#include <QSharedPointer>
#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "feature.h"
#include "tools.hpp"

namespace hsom {

/// The HuFeature class provides a feature based upon image data */
class HuFeature : public Feature
{

public:

    /// Constructs a hu feature
    HuFeature();

    /** Constructs a feature image with specified size and source image
      * @note   The image may be color or grayscale and any size
      */
    HuFeature(
        const cv::Mat& img ///< The image source for the data
        );

    /// Initializes this hu feature
    virtual void initialize();

    /// Destructs a Hu Feature
    virtual ~HuFeature();

    /*
    // Generated with training set and epsilon = 0.625
    /// @todo  figure out what the hell the above comment means

    /// Retains the alpha values for the sigmoid function
    static QVector<double> huAlpha;

    /// Retains the mean of hu values
    static QVector<double> huMean;

    /// Retains the standard deviaton of hu values
    static QVector<double> huStdv;
    /// Sets the parameters for the sigmoid correction function
    static void setSigmoidParams(
        const QVector<double>& huMean, ///< The mean values for all hu features
        const QVector<double>& huStdv, ///< The standard deviations for all hu features
        const QVector<double>& huAlpha ///< The alpha tuning parameter for all hu features
        );

    /// Fetches the parameters for the sigmoid correction function
    static void sigmoidParams(
        QVector<double>& huMean, ///< The mean values for all hu features
        QVector<double>& huStdv, ///< The standard deviations for all hu features
        QVector<double>& huAlpha ///< The alpha tuning parameter for all hu features
        ) const;

    /// Uses a sigmoid squashing function to map hu values to the range ( 0, 1 )
    static double sigmoid(
        double t, ///< The input hu value
        int idx   ///< The index of the hu value to quash
        );

QVector<double> HuFeature::huAlpha;
QVector<double> HuFeature::huMean;
QVector<double> HuFeature::huStdv;
void HuFeature::setSigmoidParams( const QVector<double> &huMean,
                                  const QVector<double> &huStdv,
                                  const QVector<double> &huAlpha )
{
    ASSERT_SET(  HuFeature::huMean,  huMean,  huMean.size()  == 7, "A Hu vector must have 7 elements" );
    ASSERT_SET(  HuFeature::huStdv,  huStdv,  huStdv.size()  == 7, "A Hu vector must have 7 elements" );
    ASSERT_SET(  HuFeature::huAlpha, huAlpha, huAlpha.size() == 7, "A Hu vector must have 7 elements" );
}

void HuFeature::sigmoidParams( QVector<double> &huMean, QVector<double> &huStdv, QVector<double> &huAlpha ) const
{
    ASSERT_SET( huMean, HuFeature::huMean,
                HuFeature::huMean.size() == 7,   "The Hu mean sigmoid parameter has not been set"  );
    ASSERT_SET( huStdv, HuFeature::huStdv,
                HuFeature::huStdv.size() == 7,   "The Hu stdv sigmoid parameter has not been set"  );
    ASSERT_SET( huAlpha, HuFeature::huAlpha,
                HuFeature::huAlpha.size() == 7,  "The Hu alpha sigmoid parameter has not been set" );
}

const QVector<double>& HuFeature::originalHus() const
{
    return huVals;
}




double HuFeature::sigmoid( double t, int idx )
{
    // Execute the sigmoid function
    return 1 / ( 1 + exp( -huAlpha[idx] * ( t - huMean[idx] ) ) );
}
        */
};

typedef QSharedPointer<HuFeature> HuFeaturePtr;

} // namespace hsom
