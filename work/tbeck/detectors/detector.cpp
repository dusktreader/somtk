#include "detector.h"

using namespace std;

Detector::Detector( const std::string& alias, DType dtype ) : isValid(false),alias(alias), dtype(dtype){}

Detector::Detector( const string& fileName, const string& ualias, DType dtype ) : alias(ualias), dtype(dtype)
{
    load( fileName );
}

Detector::~Detector(){}

cv::Mat Detector::filtered()
{
    return flt;
}

bool Detector::load( const string& fileName )
{
    /// @todo add checks for fs validity
    cv::FileStorage fs( fileName, cv::FileStorage::READ );
    if( !fs.isOpened() )
        throw DetectorException( "Couldn't open data file: " + fileName );
    cv::FileNode root = fs.getFirstTopLevelNode();
    if( root.empty() )
        throw DetectorException( "No data in root node" );
    if( root.name() != alias )
        throw DetectorException( "Detector type missmatch:  Trying to load " + alias + "; found " + root.name() );
    return read( root );
}

bool Detector::save( const string& fileName )
{
    if( isValid == false )
        return false;                                                                                                   // Can't save an invalid detector
    /// @todo add checks for fs validity
    cv::FileStorage fs( fileName, cv::FileStorage::WRITE );
    if( !fs.isOpened() )
        throw DetectorException( "Couldn't open data file: " + fileName );
    fs << alias << "{";
    write( fs );
    fs << "}";
    return true;
}

string Detector::getAliasFromFile( const std::string& fileName )
{
    cv::FileStorage fs( fileName, cv::FileStorage::READ );
    if( !fs.isOpened() )
        throw DetectorException( "Couldn't open data file: " + fileName );
    cv::FileNode root = fs.getFirstTopLevelNode();
    if( root.empty() )
        throw DetectorException( "No data in root node" );
    return root.name();
}

void Detector::checkParameters( const std::vector<double>& parameters )
{
    try
    {
        ASSERT_MSG( parameters.size() == plimits.size(), "Invalid parameter count" );
        for( unsigned int i=0; i<parameters.size(); i++ )
        {
            double p  = parameters[i],
                   lo = plimits[i].first,
                   hi = plimits[i].second;
            ASSERT_MSG( IN_CLOSED_INTERVAL( p, lo, hi ),
                        "Parameter " + num2str(i) + "=" + num2str(p) + " out of valid range=[" +
                        num2str(lo) + "," + num2str(hi) + "]" );
        }
    }
    catch( LocalAssert e )
    {
        throw DetectorException( e.message() );
    }
}

void Detector::isolateMaxima2( cv::Mat src, cv::Size detectionSize, vector<cv::Rect>& detections )
{
    int w = detectionSize.width;
    cv::Mat wrk;
    cv::copyMakeBorder( src, wrk, w, w, w, w, cv::BORDER_CONSTANT, 0 );
    RectPlus<double>  roi( detectionSize );
    PointPlus<double> currPoint;
    double maximum;
    for( currPoint.y=w; currPoint.y<wrk.size().height-w; currPoint.y+=w/2 )
    {
        for( currPoint.x=w; currPoint.x<wrk.size().width-w; currPoint.x+=w/2 )
        {
            roi.anchorOn( currPoint );
            cv::Point p;
            cv::minMaxLoc( cv::Mat( wrk, roi ), NULL, &maximum, NULL, &p );
            if( maximum > 0 )
            {
                roi.centerOn( currPoint + p );
                cv::rectangle( wrk, roi.ul(), roi.lr(), 0, CV_FILLED );
                roi.move( -w, -w );
                detections.push_back( roi );
            }
        }
    }
}

void Detector::isolateMaxima( cv::Mat src, cv::Size detectionSize, vector<cv::Rect>& detections )
{
    double maximum;
    cv::Point loc;
    cv::Rect detection( 0, 0, detectionSize.width, detectionSize.height );
    int detXOff = (int)( detection.width  * 0.5  );
    int detYOff = (int)( detection.height * 0.5  );
    int detectionCount = 0;
    while( 1 )
    {
        cv::minMaxLoc( src, NULL, &maximum, NULL, &loc );
        if( maximum == 0.0 )
            break;
        else
        {
            detection.x = loc.x - detXOff;
            detection.y = loc.y - detYOff;
            detections.push_back( detection );
            detectionCount++;
            cv::circle(  src, loc, detXOff, 0, CV_FILLED );
        }
    }
}
