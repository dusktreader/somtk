#pragma once

#include "feature.h"
#include "tools.hpp"


class ColorFeature : public Feature
{
private:
    static const std::string alias;
    static RandMaster rng;
public:
    ColorFeature();
    ColorFeature( int red, int green, int blue );
    ColorFeature( cv::Vec3b& bgr );
    virtual ~ColorFeature();

    cv::Vec3b cvColor();
};
