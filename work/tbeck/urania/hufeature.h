#pragma once

#include <QSharedPointer>
#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "feature.h"

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

    /// Destructs a Hu Feature
    virtual ~HuFeature();


    // Initializes this hu feature
    // For now, the default normalization is used
    // virtual void intialize();
};

typedef QSharedPointer<HuFeature> HuFeaturePtr;

} // namespace hsom
