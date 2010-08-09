#include "colorsuspect.h"

using namespace std;

ColorSuspect::ColorSuspect( const cv::Mat& img,
                            int realCat, int catCt,
                            const SizePlus<int>& histSz,
                            string name )
    : Suspect( name, realCat, catCt, histSz )
{
    cv::resize( img, this->img, cv::Size( 20, 20 ), 0, 0, cv::INTER_CUBIC );
}

ColorSuspect::~ColorSuspect(){}

Feature* ColorSuspect::getNextFeature()
{
    RectPlus<int> bounds( img.size() );
    while( imgPt.inside( bounds )  )
    {
        if( currFeat != NULL )
            delete currFeat;
        currFeat = new ColorFeature( img.at<cv::Vec3b>( imgPt ) );
        imgPt.x += 1;
        if( imgPt.x >= bounds.right() )
        {
            imgPt.x = 0;
            imgPt.y += 1;
        }
        if( currFeat->hasContent() )
            return currFeat;
    }
    imgPt = PointPlus<int>();
    return NULL;
}
