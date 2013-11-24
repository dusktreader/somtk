#include "image_library.h"

namespace somtk {

ImageLibrary::ImageLibrary( HistogramGrid gridTemplate, QMap< QString, QVariant > libraryParameters )
    : SuspectLibrary( gridTemplate, libraryParameters )
{}

SuspectPtr ImageLibrary::loadSuspect( QDomElement suspectElement )
{
    QString imageName = suspectElement.attribute( "path" );
    QString imagePath = libraryDir.absoluteFilePath( imageName );
    QFileInfo imageInfo( imagePath );
    SOMError::requireCondition( imageInfo.exists(), "Image doesn't exist: " + imagePath );
    SOMError::requireCondition( imageInfo.isReadable(), "Image isn't readable: " + imagePath );

    cv::Mat rawImage = cv::imread( imageInfo.absoluteFilePath().toStdString(), CV_LOAD_IMAGE_COLOR );
    SOMError::requireCondition(
                !rawImage.empty(),
                "Couldn't load image: " + imageInfo.absoluteFilePath()
                );

    ImageSuspectPtr imageSuspect = generateSuspect( rawImage );
    SuspectPtr suspect( imageSuspect );
    suspect->setName( imageName );
    return suspect;
}

} // namespace somtk
