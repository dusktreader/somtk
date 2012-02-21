#pragma once

#include <QSharedPointer>
#include <QSize>
#include <QRect>

#include "suspect.h"
#include "hufeature.h"

#include "opencv2/core/core.hpp"

namespace hsom {

class ImageSuspect : public Suspect
{

private:

    /// The image that represents the content of this suspect
    cv::Mat image;

    /// The size of the scanning window that is used to isolate featues in the source image
    QSize scanningWindowSize;

    /** The multiple of the window dimensions with which to step across the image
      * For example, a step of .25 would move the window by a quarter it's width on each step
      */
    double scanningWindowStep;



public:

    /// Creates a new ImageSuspect
    ImageSuspect(
            const cv::Mat& image,       ///< The image that this Suspect represents
            QSize   scanningWindowSize, ///< The size of the scanning window that isolates features
            double scanningWindowStep   ///< The multiple of the window dimensions with which to step across the image
            );

    /// Destructs this suspect
    virtual ~ImageSuspect();

    /// Generates the features for this supsect
    virtual void generateFeatures();
};

typedef QSharedPointer<ImageSuspect> ImageSuspectPtr;

} // namespace hsom
