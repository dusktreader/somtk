#include "suspect.h"

using namespace std;

Suspect::Suspect( string name, int realCat, int catCt, const SizePlus<int>& sz )
    : _name(name), _realCat(realCat)
{
    Feature* currFeat;
    hist = SOMHistogram( sz );
    _cats = cv::Mat_<double>( SizePlus<int>( catCt, 1 ), 0.0 );
}

Suspect::~Suspect()
{
    if( currFeat != NULL )
        delete currFeat;
}

void Suspect::setANNVectors( cv::Mat_<double>& input, cv::Mat_<double>& output )
{
    ASSERT_MSG( output.empty() || output.size() == _cats.size(), "Ouput size missmatch" );
    for( int i=0; i<hist.l(); i++ )
        input(0,i) = hist[i];
    if( output.empty() )
        return;
    output = -1.0;
    output(0,_realCat) = 1.0;
}

void Suspect::setCats( const cv::Mat_<double>& output )
{
    ASSERT_MSG( output.size() == _cats.size(), "Output size missmatch" );
    _cats = output;
}

cv::Mat_<double> Suspect::cats()
{
    return _cats;
}

int Suspect::realCat()
{
    return _realCat;
}

int Suspect::predCat()
{
    int maxIdx = -1;
    double maxVal = -DBL_MAX;
    for( int i=0; i<_cats.size().width; i++ )
    {
        if( _cats(0,i) > maxVal )
        {
            maxIdx = i;
            maxVal = _cats(0,i);
        }
    }
    return maxIdx;
}

int Suspect::catCt()
{
    return _cats.size().width;
}

void Suspect::incrementHistogram( int idx )
{
    hist.increment( idx );
}

void Suspect::incrementHistogram( const PointPlus<int>& pt )
{
    hist.increment( pt );
}

void Suspect::normalizeHistogram()
{
    hist.normalize();
}

cv::Mat Suspect::vizHistogram()
{
    return hist.vizHistogram();
}

string Suspect::name()
{
    return _name;
}
