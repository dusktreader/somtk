#include <QVector>

#include "feature.h"
#include "som.h"
#include "colorfeature.h"

#include "cvtypesplus.hpp"

int main( int argc, char *argv[] )
{
    QVector<FeaturePtr> features;

    for( int i = 0; i < 100; i++ )
        features.append( ColorFeaturePtr( new ColorFeature() ) );

    int epochCount = 10;
    double inialAlpha = .3;
    double initalRadiusRatio = .25;

    SizePlus<int> size( 25, 25 );
    SOM colorSOM( size );

    colorSOM.initializeTraining( epochCount, inialAlpha, initalRadiusRatio, features[0] );

    do
    {
        foreach( FeaturePtr feature, features )
        {
            colorSOM.train( feature );
        }
    } while( colorSOM.nextEpoch() != false );

    /// @todo  draw SOM

}
