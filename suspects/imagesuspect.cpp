#include "imagesuspect.h"

namespace somtk {

ImageSuspect::ImageSuspect( QImage image, HistogramGrid gridTemplate )
    : Suspect( gridTemplate ), _originalImage( image ), _contentThreshold( 0 )
{}

void ImageSuspect::filterImage(){}

void ImageSuspect::generateFeatures()
{
    filterImage();

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

    int windowSide = ( int )round( qMin( _originalImage.size().width(), _originalImage.size().height() ) * windowRatio );
    int windowStep = ( int )round( windowOverlap * windowSide );
    int horizontalSteps = _originalImage.size().width() / windowStep;
    int verticalSteps = _originalImage.size().height() / windowStep;
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
            if( window.right() > _originalImage.width() )
                continue;
            if( window.bottom() > _originalImage.height() )
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
