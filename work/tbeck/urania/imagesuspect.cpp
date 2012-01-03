#include "imagesuspect.h"

namespace hsom {

ImageSuspect::ImageSuspect( const cv::Mat& image, QSize scanningWindowSize, double scanningWindowStep ) :
        Suspect()
{
    this->image = image.clone();
    this->scanningWindowSize = scanningWindowSize;
    this->scanningWindowStep = scanningWindowStep;
}



ImageSuspect::~ImageSuspect(){}



void ImageSuspect::generateFeatures()
{
    int dx = scanningWindowSize.width()  * scanningWindowStep;
    int dy = scanningWindowSize.height() * scanningWindowStep;

    QRect scanningWindow( QPoint( 0, 0 ), scanningWindowSize );
    QSize imageBounds( image.size.width, image.size.height );

    while( scanningWindow.bottom() < imageBounds.height() )
    {
        while( scanningWindow.right() < imageBounds.width() )
        {
            cv::Mat subImage = image( cv::Rect( scanningWindow.left(),  scanningWindow.top(),
                                                scanningWindow.width(), scanningWindow.height() ) );

            /// @todo:  Add a test to see if the sub image is 'empty'  and throw it out if it is

            HuFeaturePtr feature( new HuFeature( subImage ) );
            _features.append( feature );

            scanningWindow.moveLeft( scanningWindow.left() + dx );
        }
        scanningWindow.moveTop( scanningWindow.top() + dy );
    }
}

} // namespace hsom
