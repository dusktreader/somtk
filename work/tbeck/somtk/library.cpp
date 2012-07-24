#include "library.h"

namespace somtk {

Library::Library()
{}



void Library::load( QString libraryXML, HistogramGrid gridTemplate )
{
    QFileInfo libraryXMLInfo( libraryXML );
    QString absPath = libraryXMLInfo.absoluteFilePath();

    SOMError::requireCondition( libraryXMLInfo.exists(), "Library file doesn't exist: " + absPath );
    SOMError::requireCondition( libraryXMLInfo.isReadable(), "Library file can't be read: " + absPath );

    QDir libraryDir = libraryXMLInfo.absoluteDir();

    QFile file( libraryXMLInfo.absoluteFilePath() );
    file.open( QFile::ReadOnly | QFile::Text );

    SOMError::requireCondition( file.isOpen(), "Failed to open library file: " + absPath );

    QDomDocument doc;

    QString domErrorText;
    SOMError::requireCondition( doc.setContent( &file, &domErrorText ), "Failed to set content for xml reader" );

    bool ok;
    QDomNode rootNode = doc.firstChild();
    QDomElement libraryElement = rootNode.firstChildElement( "library" );
    QString temp = libraryElement.attribute( "categories" );
    int categoryCount = temp.toInt( &ok );
    SOMError::requireCondition( ok, "Couldn't fetch category count for library " );

    QDomNodeList categoryNodes = libraryElement.elementsByTagName( "category" );

    for( int i = 0; i < categoryNodes.size(); i++ )
    {
        QDomElement categoryElement = categoryNodes.at( i ).toElement();

        int categoryId = categoryElement.attribute( "id" ).toInt( &ok );
        SOMError::requireCondition( ok, "Couldn't convert category id to integer" );
        SOMError::requireCondition( categoryId > 0, "Can't add a negatie category id" );
        SOMError::requireCondition( categoryId < categoryCount, "Category id greater than allowable category ids" );

        QString categoryName = categoryElement.attribute( "name" );

        _categories[ categoryId ] = categoryName;

        QDomNodeList imageNodes = categoryElement.elementsByTagName( "image" );

        for( int j = 0; j < imageNodes.size(); j++ )
        {
            QDomElement imageElement = imageNodes.at( j ).toElement();

            QString imagePath = imageElement.attribute( "path" );
            QImage image( libraryDir.absoluteFilePath( imagePath ) );
            SOMError::requireCondition( !image.isNull(), "Couldn't load image: " + imagePath );

            SuspectPtr suspect( new ColorSuspect( image, gridTemplate ) );
            suspect->setRealCategory( categoryId );
            _suspects << suspect;
        }
    }
}



QVector<SuspectPtr> Library::suspects( int category )
{
    QVector<SuspectPtr> matchingSuspects;
    if( category == -1 )
    {
        matchingSuspects = _suspects;
    }

    else
    {
        foreach( SuspectPtr suspect, _suspects )
        {
            if( suspect->realCategory() == category )
                matchingSuspects << suspect;
        }
    }

    return matchingSuspects;
}



QMap< int, QString > Library::categories()
{
    return _categories;
}

} // namespace somtk
