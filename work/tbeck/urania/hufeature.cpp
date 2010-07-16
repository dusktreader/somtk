#include "hufeature.h"

using namespace std;

string HuFeature::alias = "HuFeature";

vector<double> HuFeature::huAlpha;                                                                                      // This static data member retains the alpha values for the sigmoid function
vector<double> HuFeature::huMean;                                                                                       // This static data member retains the mean of hu values
vector<double> HuFeature::huStdv;                                                                                       // This static data member retains the standard deviaton of hu values

HuFeature::HuFeature()
{
    for( int i=0; i<7; i++ )                                                                                            // Intialize the data vector with random values
        data.push_back( RAND_DBL );
}

HuFeature::HuFeature( IplImage* img ){
    ENTER;
    l = 7;
    data = vector<double>( 7, 0.0 );                                                                                    // Create an empty data vector

    CvMoments moments;
    cvMoments( img, &moments );
    CvHuMoments huMoments;
    cvGetHuMoments( &moments, &huMoments );                                                                             // Calculate the hu moments for the input image

    huVals.push_back( huMoments.hu1 );
    huVals.push_back( huMoments.hu2 );
    huVals.push_back( huMoments.hu3 );
    huVals.push_back( huMoments.hu4 );
    huVals.push_back( huMoments.hu5 );
    huVals.push_back( huMoments.hu6 );
    huVals.push_back( fabs(huMoments.hu7) );                                                                            // The sign of the seventh hu moment should be eliminated
    if( huAlpha.size() > 0 )                                                                                            // If the sigmoid parameters have been calculated, adjust the hu moments with the sigmoid function
        for( int i=0; i<7; i++ )
            data[i] = sigmoid( huVals[i], i );

    RETURN;
}

HuFeature::~HuFeature(){
    ENTER;
    RETURN;
}

void HuFeature::setSigmoidParams( const vector<double> &huMean,
                                  const vector<double> &huStdv,
                                  const vector<double> &huAlpha ){
    ENTER;
    HuFeature::huMean = vector<double>( 7, 0.0 );
    HuFeature::huStdv = vector<double>( 7, 0.0 );
    HuFeature::huAlpha = vector<double>( 7, 0.0 );
    copy( huMean.begin(), huMean.end(), HuFeature::huMean.begin() );
    copy( huStdv.begin(), huStdv.end(), HuFeature::huStdv.begin() );
    copy( huAlpha.begin(), huAlpha.end(), HuFeature::huAlpha.begin() );
    RETURN;
}

void HuFeature::getSigmoidParams( vector<double> &huMean,
                                  vector<double> &huStdv,
                                  vector<double> &huAlpha ){
    ENTER;
    copy( HuFeature::huMean.begin(), HuFeature::huMean.end(), huMean.begin() );
    copy( HuFeature::huStdv.begin(), HuFeature::huStdv.end(), huStdv.begin() );
    copy( HuFeature::huAlpha.begin(), HuFeature::huAlpha.end(), huAlpha.begin() );
    RETURN;
}

vector<double> HuFeature::getOriginalHus(){
    ENTER;
    RETURN huVals;
}

double HuFeature::sigmoid( double t, int idx ){
    ENTER;
    double s = 1 / ( 1 + exp( -huAlpha[idx] * ( t - huMean[idx] ) ) );                                                  // Execute the sigmoid function
    RETURN s;
}
