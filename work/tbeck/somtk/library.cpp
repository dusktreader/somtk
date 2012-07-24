#include "library.h"

namespace somtk {

Library::Library( QString name ) : _name( name )
{}



void Library::load( QString libraryXML, HistogramGrid gridTemplate )
{
    bool ok;
    QString attribute;
    QImage image;
    _categoryCount = 0;

    QFileInfo libraryXMLInfo( libraryXML );
    QString absPath = libraryXMLInfo.absoluteFilePath();

    SOMError::requireCondition( libraryXMLInfo.exists(), "Library file doesn't exist: " + absPath );
    SOMError::requireCondition( libraryXMLInfo.isReadable(), "Library file can't be read: " + absPath );

    QDir libraryDir = libraryXMLInfo.absoluteDir();

    QFile file( libraryXMLInfo.absoluteFilePath() );
    file.open( QFile::ReadOnly | QFile::Text );
    SOMError::requireCondition( file.isOpen(), "Failed to open library file: " + absPath );

    QDomDocument doc( "LibraryDocument" );
    SOMError::requireCondition( doc.setContent( &file ), "Failed to set content for xml reader" );

    QDomElement rootElement = doc.documentElement();
    SOMError::requireCondition( !rootElement.isNull(), "The root node was null" );

    QDomElement libraryElement = rootElement.firstChildElement( "library" );
    SOMError::requireCondition( !libraryElement.isNull(), "Failed to fetch the library element" );

    attribute = libraryElement.attribute( "categories", "FAIL" );
    SOMError::requireCondition( attribute != "FAIL", "Failed to fetch categories attribute" );
    int categoryCount = attribute.toInt( &ok );
    SOMError::requireCondition( ok, "Couldn't convert category attribute to integer: " + attribute );

    QDomNodeList categoryNodes = libraryElement.elementsByTagName( "category" );

    for( int i = 0; i < categoryNodes.size(); i++ )
    {
        QDomElement categoryElement = categoryNodes.at( i ).toElement();

        int categoryId = categoryElement.attribute( "id" ).toInt( &ok );
        SOMError::requireCondition( ok, "Couldn't convert category id to integer" );
        SOMError::requireCondition( categoryId >= 0, "Can't add a negatie category id" );
        SOMError::requireCondition( categoryId < categoryCount, "Category id greater than allowable category ids" );
        _categoryCount = qMax( _categoryCount, categoryId + 1 );

        QString categoryName = categoryElement.attribute( "name" );

        _categories[ categoryId ] = categoryName;

        QDomNodeList imageNodes = categoryElement.elementsByTagName( "image" );

        for( int j = 0; j < imageNodes.size(); j++ )
        {
            QDomElement imageElement = imageNodes.at( j ).toElement();

            QString imageName = imageElement.attribute( "path" );
            QString imagePath = libraryDir.absoluteFilePath( imageName );
            QFileInfo imageInfo( imagePath );
            SOMError::requireCondition( imageInfo.exists(), "Image doesn't exist: " + imagePath );
            SOMError::requireCondition( imageInfo.isReadable(), "Image isn't readable: " + imagePath );
            SOMError::requireCondition( image.load( imagePath ), "Couldn't load image: " + libraryDir.absoluteFilePath( imagePath )  );

            SuspectPtr suspect( new ColorSuspect( image, gridTemplate ) );
            suspect->setRealCategory( categoryId );
            suspect->setName( imageName );
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

int Library::categoryCount()
{
    return _categoryCount;
}

QString Library::name()
{
    return _name;
}

} // namespace somtk
