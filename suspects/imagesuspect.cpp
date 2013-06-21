#include "imagesuspect.h"

namespace somtk {

ImageSuspect::ImageSuspect( cv::Mat image )
    : Suspect(), _image( image )
{}

ImageSuspect::~ImageSuspect(){}

void ImageSuspect::calibrate()
{}

void ImageSuspect::generateFeatures()
{
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

    int windowSide = ( int )round( qMin( _suspectImage.size().width(), _suspectImage.size().height() ) * windowRatio );
    int windowStep = ( int )round( windowOverlap * windowSide );
    int horizontalSteps = _suspectImage.size().width() / windowStep;
    int verticalSteps = _suspectImage.size().height() / windowStep;
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
            if( window.right() > _suspectImage.width() )
                continue;
            if( window.bottom() > _suspectImage.height() )
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
