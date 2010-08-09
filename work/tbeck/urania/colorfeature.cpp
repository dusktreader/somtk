#include "colorfeature.h"

using namespace std;

RandMaster ColorFeature::rng;

ColorFeature::ColorFeature()
    : Feature()
{
    vector<double> newData( 3 );
    newData[0] = rng.randi(0,255);
    newData[1] = rng.randi(0,255);
    newData[2] = rng.randi(0,255);
    setData( newData );
}

ColorFeature::ColorFeature( int red, int green, int blue )
{
    vector<double> newData( 3 );
    newData[0] = red;
    newData[1] = green;
    newData[2] = blue;
    setData( newData );
}

ColorFeature::ColorFeature( const cv::Vec3b& bgr )
{
    vector<double> newData( 3 );
    newData[0] = bgr[2];
    newData[1] = bgr[1];
    newData[2] = bgr[0];
    setData( newData );
}

ColorFeature::~ColorFeature(){}

cv::Vec3b ColorFeature::cvColor()
{
    return cv::Vec3b( (uchar)data[2], (uchar)data[1], (uchar)data[0] );
}

bool ColorFeature::hasContent()
{
    return data[0] > 50 && data[1] > 50 && data[2] > 50;
}

cv::Mat ColorFeature::visualize()
{
    cv::Mat viz( SizePlus<int>( 20, 20 ), CV_8UC3 );
    viz.setTo( 0 );
    cv::circle( viz, PointPlus<int>(10,10), 10, cv::Scalar( cvColor() ), CV_FILLED );
    return viz;
}
