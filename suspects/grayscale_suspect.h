#pragma once

#include <QColor>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "suspects/suspect.h"
#include "suspects/imagesuspect.h"

namespace somtk {

class GrayscaleSuspect : public ImageSuspect
{

private:

    /// @todo: doc
    double _contentThreshold;



protected:

    virtual bool hasContent( QRect window );
    virtual FeaturePtr extractFeature( QRect window );
    virtual void calibrate();



public:

    GrayscaleSuspect();
    virtual ~GrayscaleSuspect();
    virtual void setImage( cv::Mat image );

};

} // namespace somtk
