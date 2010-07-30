#include "colorsuspect.h"

using namespace std;

ColorSuspect::ColorSuspect( const cv::Mat& img,
                            int realCat, int catCt,
                            const SizePlus<int>& histSz,
                            string name )
    : Suspect( name, realCat, catCt, histSz )
{
    this->img = img.clone();
}

ColorSuspect::~ColorSuspect(){}

Feature* ColorSuspect::getNextFeature()
{
    if( currFeat != NULL )
        delete currFeat;
    currFeat = new ColorFeature( );
    RectPlus<int> bounds( img.size() );
    currFeat = new ColorFeature( img.at<cv::Vec3b>( imgPt ) );
    while( imgPt.inside( bounds ) && currFeat->empty() )
    {
        imgPt.x += 1;
        if( imgPt.x > bounds.left() )
        {
            imgPt.x = 0;
            imgPt.y += 1;
        }
    }
    if( currFeat->empty() )
    {
        imgPt = PointPlus<int>();
        return NULL;
    }
    else
        return currFeat;
}
