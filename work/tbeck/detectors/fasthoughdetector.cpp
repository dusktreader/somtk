#include "fasthoughdetector.h"

using namespace std;

const string FastHoughDetector::uniqueAlias = "FastHoughDetector";

FastHoughDetector::FastHoughDetector() : Detector(uniqueAlias,FASTHOUGH)
{
    plimits.push_back( make_pair( 4, 64 ) );
    plimits.push_back( make_pair( 0.1, 1.0 ) );
    plimits.push_back( make_pair( 1.0, 10.0 ) );
    plimits.push_back( make_pair( 1, 255 ) );
}

FastHoughDetector::FastHoughDetector( const std::string& fileName ) : Detector(fileName,uniqueAlias,FASTHOUGH)
{
    plimits.push_back( make_pair( 4, 64 ) );
    plimits.push_back( make_pair( 0.1, 1.0 ) );
    plimits.push_back( make_pair( 1.0, 10.0 ) );
    plimits.push_back( make_pair( 1, 255 ) );
}

FastHoughDetector::~FastHoughDetector(){}

bool FastHoughDetector::read( cv::FileNode &root )
{
    isValid = false;
    if( root.empty() )
        return false;
    windowWidth        =    (int)root[ "windowWidth"       ];
    windowStepScale    = (double)root[ "windowStepScale"   ];
    detectionInfluence = (double)root[ "detectionInfluence"];
    detectionThreshold =    (int)root[ "detectionThreshold"];
    prepare();
    isValid = true;
    return true;
}

bool FastHoughDetector::write( cv::FileStorage& fs )
{
    fs << "windowWidth"        << windowWidth
       << "windowStepScale"    << windowStepScale
       << "detectionInfluence" << detectionInfluence
       << "detectionThreshold" << detectionThreshold;
    return true;
}

void FastHoughDetector::setParameters( const std::vector<double>& parameters )
{
    checkParameters( parameters );

    windowWidth        = (int)parameters[0];
    windowStepScale    =      parameters[1];
    detectionInfluence =      parameters[2];
    detectionThreshold = (int)parameters[3];

    prepare();
    isValid = true;
}

void FastHoughDetector::getParameters( std::vector<double>& parameters ) const
{
    parameters.clear();
    parameters.push_back( windowWidth        );
    parameters.push_back( windowStepScale    );
    parameters.push_back( detectionInfluence );
    parameters.push_back( detectionThreshold );
}

void FastHoughDetector::prepare()
{
    thresholdWidth  = 29;    /// @todo This should probably be dependent on the detecton maximum size
    thresholdOffset = 30;    /// @todo This should probably be dependent on the variance in the input image
    thetaResolution = 13.56; /// @todo This value should be considered more carefully
    int w = windowWidth;

    // Prepare Circle Mask image
    int crcD = (int)( w * detectionInfluence );
    int crcR = crcD / 2;
    crc = cv::Mat( cv::Size(crcD,crcD), CV_8UC1, ZERO );
    cv::circle( crc, cv::Point(crcR,crcR), crcR, cv::Scalar( 1.0 ), CV_FILLED );
    crcOff = ( crcD - w ) / 2;

    rCt = (int)sqrt( w*w + w*w );
    tCt = (int)( 180 / thetaResolution );                                                                                // Describes the number of rows in the hough space matrix
    double dt  = PI / tCt;
    rhoTable = vector< vector< vector<int> > >
               ( tCt, vector< vector<int> >( w, vector<int>( w, 0 ) ) );

    double sinVal, cosVal;
    #pragma omp parallel for private( sinVal, cosVal )
    for( int t=0; t<tCt; t++ ){
        sinVal = sin( t * dt );
        cosVal = cos( t * dt );
        for( int y=0; y<w; y++ ){
            for( int x=0; x<w; x++ ){
                rhoTable[t][x][y] = (int)( x*cosVal + y*sinVal ) + rCt;
            }
        }
    }
    rCt *= 2;
}

bool FastHoughDetector::fastHough( const cv::Mat& src, cv::Mat& acc ) const
{
    acc.setTo( 0 );
    int x, y, t, r, v;
    for( y=0; y<src.rows; y++ )
    {
        for( x=0; x<src.cols; x++ )
        {
            v = src.at<uchar>( y, x );
            for( t=0; t<tCt; t++ )
            {
                r = rhoTable[t][x][y];
                int& accVal = acc.at<int>( t, r );
                accVal += v;
                if( accVal >= src.cols )
                    return true;
            }
        }
    }
    return false;
}

void FastHoughDetector::search( cv::Mat src, const cv::Size&, const cv::Size& maxSize, std::vector<cv::Rect> &detections, bool procFlt )
{

    // Initialize alias variables
    int&   w   = windowWidth;
    int    s   = max( (int)( windowWidth * windowStepScale ), 1 );

    // Initialize working images
    cv::adaptiveThreshold( src, flt, 1,
                           cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV,
                           thresholdWidth, thresholdOffset );
    cv::Mat wrk( cv::Size( src.cols + 2 * crcOff, src.rows + 2 * crcOff ), CV_8UC1 );
    cv::copyMakeBorder( flt, wrk, crcOff, crcOff, crcOff, crcOff, cv::BORDER_CONSTANT, 0 );
    flt = cv::Mat( wrk.size(), CV_8UC1, ZERO );

    cv::Rect wRoi( 0, 0, w, w );
    cv::Rect cRoi( 0, 0, crc.cols, crc.rows ) ;

    #pragma omp parallel firstprivate( wRoi, cRoi )
    {
        cv::Mat acc = cv::Mat_<int>( tCt, rCt );
        cv::Mat  wMsk;

        #pragma omp for
        for( int y=crcOff; y<src.rows-w-crcOff; y+=s )
        {
            for( int x=crcOff; x<src.cols-w-crcOff; x+=s )
            {
                wRoi.x = x;
                wRoi.y = y;
                cRoi.x = x - crcOff;
                cRoi.y = y - crcOff;
                wMsk = cv::Mat( wrk, wRoi );
                if( fastHough( wMsk, acc ) )
                    #pragma omp critical
                    {
                        cv::Mat( flt, cRoi ) += crc;
                    } //omp critical
            }
        }
    } //omp parallel

    wrk = flt;

    if( procFlt ){
        cv::Rect roi( crcOff, crcOff, src.cols, src.rows );
        cv::Mat tmp( wrk, roi );
        flt = cv::Mat( tmp.size(), CV_8UC1 );
        cv::normalize( tmp, flt, 255, 0, cv::NORM_MINMAX );
    }

    cv::threshold( wrk, wrk, detectionThreshold, 0, cv::THRESH_TOZERO );
    isolateMaxima( wrk, maxSize, detections );
}
