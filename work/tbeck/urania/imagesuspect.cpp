#include "imagesuspect.h"

using namespace std;

ImageSuspect::ImageSuspect( const cv::Mat& img, const cv::Mat& msk,
                            int realCat, int catCt,
                            const SizePlus<int>& sz,
                            const SizePlus<int>& featSz,
                            string name ) :
Suspect( name, realCat, catCt, sz )
{
    this->img = img.clone();
    roi = RectPlus<int>( featSz );
    if( msk.empty() )
        this->msk = starMask( img );
    else
        this->msk = msk.clone();
}

ImageSuspect::~ImageSuspect(){}

void ImageSuspect::resetFeatureSz( const SizePlus<int>& featSz )
{
    roi = RectPlus<int>( featSz );
}

Feature* ImageSuspect::getNextFeature()
{
    if( currFeat != NULL )
        delete currFeat;
    currFeat = NULL;
    cv::Mat imgSub, mskSub;
    RectPlus<int> bounds( img.size() );
    currFeat = HuFeature();
    while( bounds.contains( roi ) && currFeat.empty() )
    {
        mskSub = crop( msk, roi );
        if( hasContent( mskSub ) )
        {
            //imgSub = crop( img, roi );
            currFeat = new HuFeature( mskSub );
            //feat = HuFeature( imgSub );
            //feat = new ImageFeature( featW, featH, imgSub );
        }
        roi += PointPlus<int>( STEP_SIZE, 0 );
        if( roi.left() > bounds.left() )
            roi.anchorOn( PointPlus<int>( roi.y % STEP_SIZE, roi.y + STEP_SIZE ) );
    }
    if( currFeat == NULL )
        roi.anchorOn( PointPlus<int>() );
    return currFeat;
}

