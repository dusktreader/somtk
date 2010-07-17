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
}

HuFeature::HuFeature( const cv::Mat& img )
{
    data = vector<double>( 7 );
    huVals = vector<double>( 7 );
    double tdata[7];
    cv::HuMoments( cv::moments( img ), tdata );
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
    huMean  = HuFeature::huMean;
    huStdv  = HuFeature::huStdv;
    huAlpha = HuFeature::huAlpha;
    copy( HuFeature::huMean.begin(), HuFeature::huMean.end(), huMean.begin() );
    copy( HuFeature::huStdv.begin(), HuFeature::huStdv.end(), huStdv.begin() );
    copy( HuFeature::huAlpha.begin(), HuFeature::huAlpha.end(), huAlpha.begin() );
}

vector<double> HuFeature::getOriginalHus()
{
    return huVals;
}

double HuFeature::sigmoid( double t, int idx )
{
    return 1 / ( 1 + exp( -huAlpha[idx] * ( t - huMean[idx] ) ) );                                                      // Execute the sigmoid function
}
