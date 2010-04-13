#include "stardetector.h"

using namespace std;

const string StarDetector::uniqueAlias = "StarDetector";

const int StarDetector::maxSizes[] = { 1, 2, 3, 4, 6, 8, 11, 12, 16, 22, 23, 32, 45, 46, 64, 90, 128 };

StarDetector::StarDetector() : Detector(uniqueAlias,STAR)
{
    cvdetector = NULL;
    plimits.push_back( make_pair( 0,  16 ) );
    plimits.push_back( make_pair( 1, 500 ) );
    plimits.push_back( make_pair( 0,  50 ) );
    plimits.push_back( make_pair( 0,  50 ) );
    plimits.push_back( make_pair( 0, 100 ) );
}

StarDetector::StarDetector( const std::string &fileName ) : Detector(uniqueAlias,STAR)
{
    cvdetector = NULL;
    load(fileName);
    plimits.push_back( make_pair( 0,  16 ) );
    plimits.push_back( make_pair( 1, 500 ) );
    plimits.push_back( make_pair( 0,  50 ) );
    plimits.push_back( make_pair( 0,  50 ) );
    plimits.push_back( make_pair( 0, 100 ) );
}

StarDetector::~StarDetector()
{
    if( cvdetector != NULL )
        delete cvdetector;
}

bool StarDetector::read( cv::FileNode& root )
{
    isValid = false;
    if( root.empty() )
        return false;
    maxSizeIndex      = (int)root[ "maxSizeIndex"      ];
    responseThreshold = (int)root[ "responseThreshold" ];
    projectThreshold  = (int)root[ "projectThreshold"  ];
    binaryThreshold   = (int)root[ "binaryThreshold"   ];
    suppression       = (int)root[ "suppression"       ];
    prepare();
    isValid = true;
    return true;
}

bool StarDetector::write( cv::FileStorage& fs )
{
    fs << "maxSizeIndex"      << maxSizeIndex
       << "responseThreshold" << responseThreshold
       << "projectThreshold"  << projectThreshold
       << "binaryThreshold"   << binaryThreshold
       << "suppression"       << suppression;
    return true;
}

void StarDetector::setParameters( const std::vector<double>& parameters )
{
    checkParameters( parameters );
    maxSizeIndex      = (int)parameters[0];
    responseThreshold = (int)parameters[1];
    projectThreshold  = (int)parameters[2];
    binaryThreshold   = (int)parameters[3];
    suppression       = (int)parameters[4];
    prepare();
    isValid = true;
}

void StarDetector::getParameters( std::vector<double>& parameters ) const
{
    parameters.clear();
    parameters.push_back( maxSizeIndex      );
    parameters.push_back( responseThreshold );
    parameters.push_back( projectThreshold  );
    parameters.push_back( binaryThreshold   );
    parameters.push_back( suppression       );
}

void StarDetector::prepare()
{
    if( cvdetector != NULL )
        delete cvdetector;
    cvdetector = new cv::StarDetector( maxSizes[maxSizeIndex], responseThreshold, projectThreshold, binaryThreshold, suppression );
}

/// @todo  check response...
void StarDetector::search( cv::Mat src, const cv::Size &minSize, const cv::Size &maxSize, vector<cv::Rect> &detections, bool )
{
    int cOff = 2 * cvdetector->maxSize;
    cv::Mat tmp( cv::Size( src.size().width/2, src.size().height/2 ), CV_8UC1 );
    cv::pyrDown( src, tmp );
    tmp = ~tmp;
    cv::Mat wrk( cv::Size(tmp.cols+2*cOff,tmp.rows+2*cOff), CV_8UC1 );
    cv::copyMakeBorder( tmp, wrk, cOff, cOff, cOff, cOff, cv::BORDER_CONSTANT, 0 );

    vector<cv::KeyPoint> keyPoints;
    (*cvdetector)( wrk, keyPoints );
    for( unsigned int i=0; i<keyPoints.size(); i++ )
    {
        cv::Rect d;
        d.width  = (int)keyPoints[i].size * 2;
        d.height = d.width;
        d.x = (int)keyPoints[i].pt.x - d.width/2 - cOff;
        d.y = (int)keyPoints[i].pt.y - d.height/2 - cOff;
        d.x *= 2;
        d.y *= 2;
        d.width *= 2;
        d.height *= 2;
        d.width  = max( min( d.width,  maxSize.width  ), minSize.width  );
        d.height = max( min( d.height, maxSize.height ), minSize.height );
        detections.push_back( d );
    }
}
