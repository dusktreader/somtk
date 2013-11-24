#include "imagesuspect.h"

namespace somtk {

ImageSuspect::ImageSuspect()
    : Suspect()
{}

ImageSuspect::~ImageSuspect(){}

void ImageSuspect::calibrate()
{}

void ImageSuspect::setImage( cv::Mat image )
{
    _image = image;
}

cv::Mat ImageSuspect::image()
{
    return _image;
}

void ImageSuspect::generateFeatures()
{
    SOMError::requireCondition( _image.empty() == false, "Cannot generate features with an empty image" );
    calibrate();

    // These will be parametrically defined soon
    double windowRatio = 0.1;
    SOMError::requireCondition(
                windowRatio < 1.0,
                QString( "Window ratio must be less than 1.0. was: %1" )
                .arg( windowRatio )
                );

    double windowOverlap = 0.5;
    SOMError::requireCondition(
                windowOverlap >= 0.0 && windowOverlap < 1.0,
                QString( "Window overlap must be between 0.0 and 1.0. was: %1" )
                .arg( windowOverlap )
                );

    int imageWidth = _image.cols;
    int imageHeight = _image.rows;
    int windowSide = ( int )round( qMin( imageWidth, imageHeight ) * windowRatio );
    int windowStep = ( int )round( windowOverlap * windowSide );
    int horizontalSteps = imageWidth / windowStep;
    int verticalSteps = imageHeight / windowStep;
    int N = horizontalSteps * verticalSteps;

    _features = QVector<FeaturePtr>();
    #pragma omp parallel
    {
        QVector<FeaturePtr> localFeatures;

        #pragma omp for
        for( int i = 0; i < N; i++ )
        {
            int x = i % horizontalSteps * windowStep;
            int y = i / horizontalSteps * windowStep;

            QRect window( QPoint( x, y ), QSize( windowSide, windowSide ) );
            if( window.right() > imageWidth )
                continue;
            if( window.bottom() > imageHeight )
                continue;

            if( hasContent( window ) )
                localFeatures << extractFeature( window );
        }

        #pragma omp critical
        {
            _features << localFeatures;
        }
    }

}

} // namespace somtk
