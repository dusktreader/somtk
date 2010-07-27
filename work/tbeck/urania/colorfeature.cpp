#include "colorfeature.h"

using namespace std;

RandMaster ColorFeature::rng;

ColorFeature::ColorFeature()
{
    for( int i=0; i<3; i++ )
        data.push_back( rng.randi(0,255) );
}

ColorFeature::ColorFeature( int red, int green, int blue )
{
    data.push_back( red );
    data.push_back( green );
    data.push_back( blue );
}

ColorFeature::ColorFeature( const cv::Vec3b& bgr )
{
    data.push_back( bgr[2] );
    data.push_back( bgr[1] );
    data.push_back( bgr[0] );
}

ColorFeature::~ColorFeature(){}

cv::Vec3b ColorFeature::cvColor()
{
    return cv::Vec3b( data[2], data[1], data[0] );
}
