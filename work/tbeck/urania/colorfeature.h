#pragma once

#include "feature.h"

#include "tools.hpp"
#include "cvtypesplus.hpp"

#include "cv.h"
#include "cxcore.h"

class ColorFeature : public Feature
{
private:
    static const std::string alias;
    static RandMaster rng;
public:
    ColorFeature();
    ColorFeature( int red, int green, int blue );
    ColorFeature( const cv::Vec3b& bgr );
    virtual ~ColorFeature();

    /** Determines if this feature has any content */
    virtual bool hasContent();

    /** Creates a visualization of the feature */
    cv::Mat virtual visualize();

    cv::Vec3b cvColor();
};
