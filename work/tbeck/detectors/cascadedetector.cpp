#include "cascadedetector.h"

using namespace std;

const string CascadeDetector::uniqueAlias = "cascade";

CascadeDetector::CascadeDetector() : Detector(uniqueAlias,CASCADE)
{
    plimits.push_back( make_pair( 1.01, 1.5 ) );
    plimits.push_back( make_pair( 1, 50 ) );
}

CascadeDetector::CascadeDetector( const std::string &fileName ) : Detector(uniqueAlias,CASCADE)
{
    load( fileName );
    plimits.push_back( make_pair( 1.01, 1.5 ) );
    plimits.push_back( make_pair( 1, 50 ) );
}

CascadeDetector::~CascadeDetector(){}

void CascadeDetector::prepare(){}

#ifdef STARTRACK_CUSTOM
void CascadeDetector::search( cv::Mat src, const cv::Size &minSize, const cv::Size &maxSize, vector<cv::Rect> &detections, bool )
{
    detectMultiScale( src, detections, scaleFactor, minNeighbors, 0, minSize, maxSize );
}
#else
void CascadeDetector::search( cv::Mat src, const cv::Size &minSize, const cv::Size &, vector<cv::Rect> &detections, bool )
{
    detectMultiScale( src, detections, scaleFactor, minNeighbors, 0, minSize );
}
#endif

bool CascadeDetector::load( const string& fileName )
{
    if( !CascadeClassifier::load( fileName ) )
        throw DetectorException( "Couldn't load Cascade Detector: " + fileName );
    return true;
}

bool CascadeDetector::read( cv::FileNode& root )
{
    if( !CascadeClassifier::read( root ) )
        throw DetectorException( "Couldn't read Cascade Detector" );
    return true;
}

bool CascadeDetector::write( cv::FileStorage& )
{
    throw DetectorException( "Save capability not yet implemented for Cascade Detector" );
    return true;
}


void CascadeDetector::setParameters( const std::vector<double>& parameters )
{
    checkParameters( parameters );
    scaleFactor  =      parameters[0];
    minNeighbors = (int)parameters[1];
    isValid = true;
}

void CascadeDetector::getParameters( std::vector<double>& parameters ) const
{
    parameters.clear();
    parameters.push_back( scaleFactor );
    parameters.push_back( minNeighbors );
}
