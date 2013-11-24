#include "sobel_hu_library.h"

namespace somtk {

SobelHuLibrary::SobelHuLibrary( HistogramGrid gridTemplate, QMap< QString, QVariant > libraryParameters )
    : ImageLibrary( gridTemplate, libraryParameters )
{}

SobelHuLibrary::~SobelHuLibrary()
{}

ImageSuspectPtr SobelHuLibrary::generateSuspect( cv::Mat rawImage )
{
    /// @todo parameterize the shit here
    ImageSuspectPtr suspect( new GrayscaleSuspect() );

    cv::Mat filteredImage;
    cv::cvtColor( rawImage, filteredImage, CV_RGB2GRAY );
    cv::Laplacian( filteredImage, filteredImage, CV_64F, 7 );
    cv::normalize( filteredImage, filteredImage, 0.0, 1.0, cv::NORM_MINMAX );
    suspect->setImage( filteredImage );
    return suspect;
}

} // namespace somtk
