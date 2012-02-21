#include "hufeature.h"

namespace hsom {

HuFeature::HuFeature() :
        Feature( QVector<double>( 7, 0.0 ) )
{}



HuFeature::HuFeature( const cv::Mat& img ) :
        Feature( QVector<double>( 7, 0.0 ) )
{
    cv::HuMoments( cv::moments( img ), this->data() );

    // The sign of the seventh hu moment should be eliminated
    *this[6] = abs( *this[6] );
}



HuFeature::~HuFeature(){}



FeaturePtr HuFeature::manufacture()
{
    return FeaturePtr( new HuFeature() );
}

} // namespace hsom
