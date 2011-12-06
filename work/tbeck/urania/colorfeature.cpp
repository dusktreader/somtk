#include "colorfeature.h"

using namespace std;

RandMaster ColorFeature::rng;

ColorFeature::ColorFeature()
    : Feature()
{
    vector<double> newData( 3 );
    newData[0] = rng.randi(0,255);
    newData[1] = rng.randi(0,255);
    newData[2] = rng.randi(0,255);
    setData( newData );
}

ColorFeature::ColorFeature( double red, double green, double blue )
{
    vector<double> newData( 3 );
    newData[0] = red;
    newData[1] = green;
    newData[2] = blue;
    setData( newData );
}
