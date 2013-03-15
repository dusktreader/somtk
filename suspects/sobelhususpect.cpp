#include "sobelhususpect.h"

namespace somtk {

int SobelHuSuspect::dummyCounter = 0;

SobelHuSuspect::SobelHuSuspect( QImage image, HistogramGrid gridTemplate)
    : ImageSuspect( image, gridTemplate )
{}

bool SobelHuSuspect::hasContent( QRect window )
{
    // If the window would fall off the side of the image, just return false
    if( window.right() >= _filteredImage.size().width  || window.bottom() >= _filteredImage.size().height )
        return false;

    cv::Mat_<double> subImage = _filteredImage( cv::Rect( window.x(), window.y(), window.width(), window.height() ) );
    cv::Mat mask( subImage.size(), CV_8UC1, cv::Scalar( 0 ) );
    cv::Point center( window.width() / 2, window.height() / 2 );
    int radius = window.width() / 4;
    cv::circle( mask, center, radius, cv::Scalar( 255 ), CV_FILLED );
    cv::Scalar mean, stdv;
    cv::meanStdDev( subImage, mean, stdv, mask );
    double val = stdv[0];
    if( val > _contentThreshold )
    {
        QString featureName = QString( "ouput/%1_%2_%3.png" ).arg( _name ).arg( window.x() ).arg( window.y() );
        imageCVdbl2Qrgb( subImage ).save( featureName );
        return true;
    }
    else
        return false;
}

FeaturePtr SobelHuSuspect::extractFeature( QRect window )
{
    cv::Mat_<double> subImage = _filteredImage( cv::Rect( window.x(), window.y(), window.width(), window.height() ) );
    FeaturePtr feature = FeaturePtr( new Feature( 7 ) );
    cv::HuMoments( cv::moments( subImage ), feature->data() );
    return feature;
}

void SobelHuSuspect::filterImage()
{
    _filteredImage = imageQrgb2CVdbl( _originalImage );

    // Filter the image
    cv::Laplacian( _filteredImage, _filteredImage, CV_64F, 7 );
    cv::normalize( _filteredImage, _filteredImage, 0.0, 1.0, cv::NORM_MINMAX );

    // Compute the content threshold
    cv::Scalar mean, stdv;
    cv::meanStdDev( _filteredImage, mean, stdv );

    _contentThreshold = stdv[0];
}

} // namespace somtk
