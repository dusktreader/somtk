#include "sobel_hu_library.h"

namespace somtk {

SobelHuLibrary( HistogramGrid gridTemplate, QMap< QString, QVariant > libraryParameters )
    : ImageLibrary( gridTemplate, libraryParameters )
{}

SobelHuLibrary::~SobelHuLibrary()
{}

SuspectPtr SobelHuLibrary::generateSuspect( QImage rawImage )
{
    cv::Mat filteredImage = imageQrgb2CVdbl( rawImage );

    /// @todo parameterize the shit here
    // Filter the image
    cv::Laplacian( filteredImage, filteredImage, CV_64F, 7 );
    cv::normalize( filteredImage, filteredImage, 0.0, 1.0, cv::NORM_MINMAX );


    SuspectPtr suspect( new GrayscaleSuspect( filteredImage ) );
    return suspect;
}

} // namespace somtk
