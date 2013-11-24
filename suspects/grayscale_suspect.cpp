#include "grayscale_suspect.h"

namespace somtk {

GrayscaleSuspect::GrayscaleSuspect()
    : _contentThreshold( 0.0 )
{}

GrayscaleSuspect::~GrayscaleSuspect()
{}

void GrayscaleSuspect::calibrate()
{
    // Compute the content threshold
    cv::Scalar mean, stdv;
    cv::meanStdDev( _image, mean, stdv );

    _contentThreshold = stdv[0];
}

bool GrayscaleSuspect::hasContent( QRect window )
{
    // If the window would fall off the side of the image, just return false
    if( window.right() >= _image.size().width  || window.bottom() >= _image.size().height )
        return false;

    cv::Mat_<double> subImage = _image( cv::Rect( window.x(), window.y(), window.width(), window.height() ) );
    cv::Mat mask( subImage.size(), CV_8UC1, cv::Scalar( 0 ) );
    cv::Point center( window.width() / 2, window.height() / 2 );
    int radius = window.width() / 4;
    cv::circle( mask, center, radius, cv::Scalar( 255 ), CV_FILLED );
    cv::Scalar mean, stdv;
    cv::meanStdDev( subImage, mean, stdv, mask );
    double val = stdv[0];
    if( val > _contentThreshold )
    {
        // QString featureName = QString( "ouput/%1_%2_%3.png" ).arg( _name ).arg( window.x() ).arg( window.y() );
        // imageCVdbl2Qrgb( subImage ).save( featureName );
        return true;
    }
    else
        return false;
}

FeaturePtr GrayscaleSuspect::extractFeature( QRect window )
{
    cv::Mat_<double> subImage = _image( cv::Rect( window.x(), window.y(), window.width(), window.height() ) );
    FeaturePtr feature = FeaturePtr( new Feature( 7 ) );
    cv::HuMoments( cv::moments( subImage ), feature->data() );
    return feature;
}

void GrayscaleSuspect::setImage( cv::Mat image )
{
    /// @todo:  Error checking to make sure that it is indeed a grayscale image
    ImageSuspect::setImage( image );
}

} // namespace somtk
