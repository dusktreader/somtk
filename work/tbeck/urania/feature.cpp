#include "feature.h"

using namespace std;

const string Feature::alias = "Feature";

Feature::Feature()
{
    _empty = true;
}

Feature::~Feature(){}

void Feature::setData( const vector<double> &newData )
{
    data = newData;
    _empty = false;
    return;
}

double Feature::dist( const Feature& other )
{
    double d = 0.0;
    for( int i=0; i<l(); i++ )
        d += pow( data[i] - other.data[i], 2 );
    return d;
}

void Feature::adjust( const Feature& other, double scaleFactor )
{
    #pragma omp parallel for
    for( int i=0; i<l(); i++ )
        data[i] += ( other.data[i] - data[i] ) * scaleFactor;
}

int Feature::l()
{
    return data.size();
}

bool Feature::empty()
{
    return _empty;
}

void Feature::read( const cv::FileNode& fn )
{
    fn[alias] >> data;
}

void Feature::write( cv::FileStorage& fs )
{
    fs << alias << "[" << data << "]";
}
