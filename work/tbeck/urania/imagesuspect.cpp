#include "imagesuspect.h"

using namespace std;

ImageSuspect::ImageSuspect( const cv::Mat& img,
                            int realCat, int catCt,
                            const SizePlus<int>& sz,
                            const SizePlus<int>& featSz,
                            int stepSz,
                            string name )
    : Suspect( name, realCat, catCt, sz )
{
    this->img = img.clone();
    roi = RectPlus<int>( featSz );
    this->stepSz = stepSz;
}

ImageSuspect::~ImageSuspect(){}

void ImageSuspect::setFeatureSz( const SizePlus<int>& featSz )
{
    roi = RectPlus<int>( featSz );
}

Feature* ImageSuspect::getNextFeature()
{
    if( currFeat != NULL )
        delete currFeat;
    RectPlus<int> bounds( img.size() );
    currFeat = new HuFeature( crop( msk, roi ) );
    while( bounds.contains( roi ) && currFeat->empty() )
    {
        roi += PointPlus<int>( stepSz, 0 );
        if( roi.left() > bounds.left() )
            roi.anchorOn( PointPlus<int>( roi.y % stepSz, roi.y + stepSz ) );
    }
    if( currFeat->empty() )
    {
        roi.anchorOn( PointPlus<int>() );
        return NULL;
    }
    else
        return currFeat;
}

