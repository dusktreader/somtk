#include "tophatdetector.h"

using namespace std;

const string TophatDetector::uniqueAlias = "TopHatDetector";

TophatDetector::TophatDetector() : Detector(uniqueAlias,TOPHAT)
{
    plimits.push_back( make_pair(  5.0, 200.0 ) );
    plimits.push_back( make_pair(  1.0, 100.0 ) );
    plimits.push_back( make_pair(  0.1,   3.0 ) );
    plimits.push_back( make_pair(  1.0,  20.0 ) );
}

TophatDetector::TophatDetector( const std::string &fileName ) : Detector(fileName,uniqueAlias,TOPHAT)
{
    plimits.push_back( make_pair(  5.0, 200.0 ) );
    plimits.push_back( make_pair(  1.0, 100.0 ) );
    plimits.push_back( make_pair(  0.1,   3.0 ) );
    plimits.push_back( make_pair(  1.0,  20.0 ) );
}

TophatDetector::~TophatDetector(){}

bool TophatDetector::read( cv::FileNode& root ){
    isValid = false;
    if( root.empty() )
        return false;
    crownRadius =    (int)root[ "crownRadius" ];
    brimWidth   =    (int)root[ "brimWidth"   ];
    hatHeight   = (double)root[ "hatHeight"   ];
    stepSize    =    (int)root[ "stepSize"    ];
    prepare();
    isValid = true;
    return true;
}

bool TophatDetector::write( cv::FileStorage& fs ){
    fs << "crownRadius" << crownRadius
       << "brimWidth"   << brimWidth
       << "hatHeight"   << hatHeight
       << "stepSize"    << stepSize;
    return true;
}

void TophatDetector::setParameters( const std::vector<double>& parameters ){
    this->checkParameters( parameters );
    crownRadius        = (int)parameters[0];
    brimWidth          = (int)parameters[1];
    hatHeight          =      parameters[2];
    stepSize           = (int)parameters[3];
    prepare();
    isValid = true;
}

void TophatDetector::getParameters( std::vector<double>& parameters ) const{
    parameters.clear();
    parameters.push_back( crownRadius );
    parameters.push_back( brimWidth   );
    parameters.push_back( hatHeight   );
    parameters.push_back( stepSize    );
}

void TophatDetector::prepare(){
    int brimRadius = crownRadius + brimWidth;
    int w = 2 * brimRadius;
    crownMask = cv::Mat( cv::Size( w, w ), CV_8UC1 );
    crownMask.setTo( 0 );
    brimMask = crownMask.clone();
    cv::Point c( brimRadius, brimRadius );
    cv::circle( crownMask, c, crownRadius, 1, CV_FILLED );
    cv::circle( brimMask,  c, brimRadius,  1, CV_FILLED );
    cv::circle( brimMask,  c, crownRadius, 0, CV_FILLED );
}

uchar TophatDetector::calcHat( const cv::Mat& src, const cv::Rect& roi ){
    cv::Mat tmp( src, roi );
    cv::Scalar brimStdv, brimMean, crownMean;
    cv::meanStdDev( tmp, brimMean, brimStdv, brimMask );
    crownMean = cv::mean( tmp, crownMask );
    double diff = crownMean[0] - brimMean[0];
    return diff > hatHeight * brimStdv[0] ? (uchar)diff : 0;
}

void TophatDetector::search( cv::Mat src, const cv::Size&, const cv::Size &maxSize, std::vector<cv::Rect> &detections, bool procFlt ){

    int brimRadius = crownRadius + brimWidth;

    cv::Mat wrk = src.clone();
    wrk = ~wrk; /// @todo algorithmically determine if the image is bright or dark background

    flt = cv::Mat( wrk.size(), CV_8U );
    flt.setTo( 0 );

    cv::Rect roi( 0, 0, 2*brimRadius, 2*brimRadius );

    #pragma omp parallel for firstprivate( roi )
    for( int y=0; y < src.size().height - roi.height; y+=stepSize ){
        for( int x=0; x < src.size().width - roi.width; x+=stepSize ){
            roi.x = x;
            roi.y = y;
            flt.at<uchar>( y+brimRadius, x+brimRadius ) = calcHat( wrk, roi );
        }
    }

    wrk = flt;

    if( procFlt ){
        flt = wrk.clone();
        cv::dilate( flt, flt, cv::Mat(), cv::Point( -1, -1 ), stepSize/2-1 );
        cv::pow( flt, 2, flt );
    }

    isolateMaxima2( wrk, maxSize, detections );

}

