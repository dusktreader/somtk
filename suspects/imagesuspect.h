#pragma once

#include "opencv2/core/core.hpp"

#include "suspects/suspect.h"
#include "errors/somerror.h"

namespace somtk {

class ImageSuspect : public Suspect
{

protected:

    cv::Mat _image;
    virtual void generateFeatures();

    /// Calibrate the suspect for determining if windows have content
    virtual void calibrate();

    /// Indicates whether an image window has any content from which to generate a feature
    virtual bool hasContent(
            QRect window /// The image window in which to check for content
            ) = 0;

    /// Extracts a feature from the given window
    virtual FeaturePtr extractFeature(
            QRect window /// The image window from which to extract a feature
            ) = 0;


public:

    ImageSuspect();
    virtual ~ImageSuspect();
    void setImage( cv::Mat image );
    cv::Mat image();
};

typedef QSharedPointer< ImageSuspect > ImageSuspectPtr;

} // namespace somtk
