#include "suspect_library.h"

namespace somtk {

SuspectLibrary::SuspectLibrary( HistogramGrid gridTemplate, QMap<QString, QVariant> libraryParameters )
    : _gridTemplate( gridTemplate )
{
    /// @todo error checking for the name library parameter
    SOMError::requireCondition(
                libraryParameters.contains( "name" ),
                "Library parameters must contain a name field"
    );
    this->_name = libraryParameters["name"].toString();
    SOMError::requireCondition(
                this->_name != "",
                "Suspect Library name may not be empty"
    );
}

SuspectLibrary::~SuspectLibrary(){}

void ImageLibrary::load( QString libraryXML )
{
    bool ok;
    QString attribute;
    _categoryCount = 0;

    QFileInfo libraryXMLInfo( libraryXML );
    QString absPath = libraryXMLInfo.absoluteFilePath();

    SOMError::requireCondition( libraryXMLInfo.exists(), "Library file doesn't exist: " + absPath );
    SOMError::requireCondition( libraryXMLInfo.isReadable(), "Library file can't be read: " + absPath );

    libraryDir = libraryXMLInfo.absoluteDir();

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

        int categoryId = categoryElement.attribute( "ikd" ).toInt( &ok );
        SOMError::requireCondition( ok, "Couldn't convert category id to integer" );
        SOMError::requireCondition( categoryId >= 0, "Can't add a negatie category id" );
        SOMError::requireCondition( categoryId < categoryCount, "Category id greater than allowable category ids" );
        _categoryCount = qMax( _categoryCount, categoryId + 1 );

        QString categoryName = categoryElement.attribute( "name" );

        _categories[ categoryId ] = categoryName;

        QDomNodeList suspectNodes = categoryElement.elementsByTagName( "suspects" );

        for( int j = 0; j < suspectNodes.size(); j++ )
        {
            QDomElement suspectElement = suspectNodes.at( j ).toElement();

            SuspectPtr suspect = loadSuspect( suspectElement );
            HistogramPtr histogram( new Histogram( gridTemplate ) );
            suspect->setHistogram( histogram );

            _suspects << suspect;
        }
    }
}



QMap< int, QString > SuspectLibrary::categories()
{
    return _categories;
}

int SuspectLibrary::categoryCount()
{
    return _categoryCount;
}

QString Library::name()
{
    return _name;
}

} // namespace somtk
