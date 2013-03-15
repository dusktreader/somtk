#include "colorsuspect.h"

namespace somtk {

ColorSuspect::ColorSuspect( QImage image, HistogramGrid gridTemplate )
    : Suspect( gridTemplate ), _image( image )
{}


void ColorSuspect::generateFeatures()
{
    int steps = 4;
    int N = steps * steps;
    QSize windowSize( _image.size().width() / steps, _image.size().height() / steps );
    _features = QVector<FeaturePtr>( N );
    #pragma omp parallel for
    for( int i = 0; i < N; i++ )
    {
        int x = i % steps;
        int y = i / steps;
        QRect window( QPoint( x, y ), windowSize );
        QImage subImage = _image.copy( window );
        QVector<double> meanColor( 3, 0.0 );
        int n = 0;
        for( y = 0; y < window.height(); y++ )
        {
            for( x = 0; x < window.width(); x++ )
            {
                QRgb currentColor = subImage.pixel( x, y );

                meanColor[0] += qRed( currentColor );
                meanColor[1] += qGreen( currentColor );
                meanColor[2] += qBlue( currentColor );
                n++;
            }
        }
        meanColor[0] /= (double)n;
        meanColor[1] /= (double)n;
        meanColor[2] /= (double)n;

        _features[i] = FeaturePtr( new Feature( meanColor ) );
    }
}

} // namespace somtk
