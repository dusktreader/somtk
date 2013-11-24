#pragma once

#include <QString>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QList>
#include <QVariant>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "errors/somerror.h"
#include "suspects/suspect.h"
#include "histograms/histogram.h"

namespace somtk {

class SuspectLibrary
{

private:

    /// A mapping of category ids to string descriptions
    QMap< int, QString > _categories;

    /// The collection of suspects that are contained within this library
    QVector< SuspectPtr > _suspects;

    /// The name of this Library
    QString _name;

    /// The number of categories in this library
    int _categoryCount;



protected:

    /// A template of a histogram grid that will be provided to the suspects in this library
    HistogramGrid _gridTemplate;

    /// The directory that contained the library description file from which this was loaded
    /// @note  This is important if the paths for the suspects are relative paths
    QDir libraryDir;



public:

    /// Constructs a Library of Suspects
    SuspectLibrary(
            HistogramGrid gridTemplate,                 /// A Histogram grid template to provide to the suspects
            QMap< QString, QVariant > libraryParameters /// Additional parameters for the library
            );

    /// Destroys a Library
    virtual ~SuspectLibrary();

    /// Loads a library of suspects from a given library description file
    void load(
            QString libraryXML /// The xml document file for the library
            );

    virtual SuspectPtr loadSuspect(
            QDomElement suspectElement /// The xml element containing suspect information
            ) = 0;

    /// Fetches a mapping of the categories for this library
    QMap< int, QString > categories();

    /// Fetches the number of categories in this library
    int categoryCount();

    /// Fetches the name of this library
    QString name();

    /// Extracts a vector of features from the suspects in this library
    QVector<FeaturePtr> extractFeatures();

    /// Fetches the collection of suspects in this library
    QVector<SuspectPtr> suspects();
};

typedef QSharedPointer< SuspectLibrary > SuspectLibraryPtr;

}
