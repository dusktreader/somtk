#include "feature.h"

using namespace std;

Feature::~Feature(){}

void Feature::setData( const vector<double> &newData )
{
    data = newData;
    return;
}

double Feature::dist( const Feature& other )
{
    double d = 0.0;
    for( int i=0; i<l(); i++ )
        d += pow( data[i] - other.data[i], 2 );
    return d;
}

void Feature::adjust( const Feature* other, double scaleFactor )
{
    #pragma omp parallel for
    for( int i=0; i<l(); i++ )
        data[i] += ( other->data[i] - data[i] ) * scaleFactor;
}

int Feature::l()
{
    return data.size();
}

void Feature::read( CvFileStorage* fs, CvFileNode* node ){
    ENTER;
    node = cvGetFileNodeByName( fs, node, FEATURE_ALIAS );
    double* fdata = new double[l];
    cvReadRawData( fs, node, fdata, "d" );
    copy( fdata, fdata+l, data.begin() );
    delete [] fdata;
    RETURN;
}

void Feature::write( CvFileStorage* fs ){
    ENTER;
    cvStartWriteStruct( fs, FEATURE_ALIAS, CV_NODE_SEQ );
    double* fdata = new double[l];
    copy( data.begin(), data.end(), fdata );
    cvWriteRawData( fs, fdata, l, "d" );
    cvEndWriteStruct( fs );
    delete [] fdata;
    RETURN;
}
