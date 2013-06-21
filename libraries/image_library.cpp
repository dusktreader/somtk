#include "image_library.h"

namespace somtk {

ImageLibrary::ImageLibrary( HistogramGrid gridTemplate, QMap< QString, QVariant > libraryParameters )
    : SuspectLibrary( gridTemplate, libraryParameters )
{}

void ImageLibrary::loadSuspect( QDomElement suspectElement )
{
    QString imageName = suspectElement.attribute( "path" );
    QString imagePath = libraryDir.absoluteFilePath( imageName );
    QFileInfo imageInfo( imagePath );
    SOMError::requireCondition( imageInfo.exists(), "Image doesn't exist: " + imagePath );
    SOMError::requireCondition( imageInfo.isReadable(), "Image isn't readable: " + imagePath );

    QImage rawImage( imageInfo.absoluteFilePath() );
    SOMError::requireCondition(
                !rawImage.isNull(),
                "Couldn't load image: " + imageInfo.absoluteFilePath()
                );

    SuspectPtr suspect = generateSuspect( rawImage );

    /// @todo move these into the base class?
    suspect->setRealCategory( categoryId );
    suspect->setName( imageName );
}


// namespace somtk
