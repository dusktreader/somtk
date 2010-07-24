#include "hufeature.h"

using namespace std;

string HuFeature::alias = "HuFeature";

RandMaster rng;

vector<double> HuFeature::huAlpha;                                                                                      // This static data member retains the alpha values for the sigmoid function
vector<double> HuFeature::huMean;                                                                                       // This static data member retains the mean of hu values
vector<double> HuFeature::huStdv;                                                                                       // This static data member retains the standard deviaton of hu values

HuFeature::HuFeature()
{
    for( int i=0; i<7; i++ )                                                                                            // Intialize the data vector with random values
        data.push_back( rng.randd() );
    _empty = false;
}

HuFeature::HuFeature( const cv::Mat& img )
{
    data = vector<double>( 7, 0.0 );
    huVals = vector<double>( 7, 0.0 );

    if( 0/* some test for valid values inside of img */ )
    {
        //bool hasContent( IplImage* src ){
        //    ENTER;
        //    if( cvGetReal2D( src, src->width / 2, src->height / 2 ) == 0 ){                                                     // If the center pixel is zero, there is no content
        //        RETURN false;
        //    }
        //    IplImage* msk = createMask( src,
        //                                cvRect( 1, 1, src->width-2, src->height-2 ),
        //                                true );                                                                                 // This will create a ring mask around the border of the image
        //    double minVal, maxVal;
        //    cvMinMaxLoc( src, &minVal, &maxVal, NULL, NULL, msk );                                                              // Find the maximum and minimum pixel values along the border
        //    cvReleaseImage( &msk );
        //    if( minVal == 0 && maxVal != 0 ){                                                                                   // 	If there is at least one zero pixel and one non-zero pixel on the border, then the image has content
        //        RETURN true;
        //    }
        //    else{
        //        RETURN false;
        //    }
        //}
        _empty = true;
        return;
    }
    else
    {
        _empty = false;
    }

    double tdata[7];
    cv::HuMoments( cv::moments( img ), data.data() );
    for( int i=0; i<7; i++ )
    {
        double val = i == 7 ? abs( tdata[i] ) : tdata[i];                                                               // The sign of the seventh hu moment should be eliminated
        huVals[i] = val;
        data[i] = huAlpha.size() > 0 ? sigmoid( val, i ) : 0;                                                           // If the sigmoid parameters have been calculated, adjust the hu moments with the sigmoid function
    }
}

HuFeature::~HuFeature(){}

void HuFeature::setSigmoidParams( const vector<double> &huMean,
                                  const vector<double> &huStdv,
                                  const vector<double> &huAlpha )
{
    ASSERT_SET(  HuFeature::huMean,  huMean,  huMean.size() == 7, "A Hu vector must have 7 elements" );
    ASSERT_SET(  HuFeature::huStdv,  huStdv,  huStdv.size() == 7, "A Hu vector must have 7 elements" );
    ASSERT_SET( HuFeature::huAlpha, huAlpha, huAlpha.size() == 7, "A Hu vector must have 7 elements" );
}

void HuFeature::getSigmoidParams( vector<double> &huMean, vector<double> &huStdv, vector<double> &huAlpha )
{
    ASSERT_SET( huMean, HuFeature::huMean,
                HuFeature::huMean.size() == 7,   "The Hu mean sigmoid parameter has not been set"  );
    ASSERT_SET( huStdv, HuFeature::huStdv,
                HuFeature::huStdv.size() == 7,   "The Hu stdv sigmoid parameter has not been set"  );
    ASSERT_SET( huAlpha, HuFeature::huAlpha,
                HuFeature::huAlpha.size() == 7,  "The Hu alpha sigmoid parameter has not been set" );
    //copy( HuFeature::huMean.begin(), HuFeature::huMean.end(), huMean.begin() );
    //copy( HuFeature::huStdv.begin(), HuFeature::huStdv.end(), huStdv.begin() );
    //copy( HuFeature::huAlpha.begin(), HuFeature::huAlpha.end(), huAlpha.begin() );
}

vector<double> HuFeature::getOriginalHus()
{
    return huVals;
}

double HuFeature::sigmoid( double t, int idx )
{
    return 1 / ( 1 + exp( -huAlpha[idx] * ( t - huMean[idx] ) ) );                                                      // Execute the sigmoid function
}

