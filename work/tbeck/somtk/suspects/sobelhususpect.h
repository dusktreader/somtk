#pragma once

#include <QColor>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "suspects/suspect.h"
#include "suspects/imagesuspect.h"
#include "tools/cvq/cvq.h"

namespace somtk {

class SobelHuSuspect : public ImageSuspect
{

protected:
    static int dummyCounter;

    cv::Mat_<double> _filteredImage;

    virtual bool hasContent( QRect window );
    virtual FeaturePtr extractFeature( QRect window );
    virtual void filterImage();

public:
    SobelHuSuspect( QImage image, HistogramGrid gridTemplate );

};

} // namespace somtk
