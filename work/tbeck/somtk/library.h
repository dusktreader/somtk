#pragma once

#include <QString>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "errors/somerror.h"
#include "suspects/colorsuspect.h"
#include "histograms/histogram.h"

namespace somtk {

class Library
{

private:

    /// A mapping of category ids to string descriptions
    QMap< int, QString > _categories;

    QVector< SuspectPtr > _suspects;

public:

    /// Constructs a Library of Suspects
    Library();

    /// Loads a library of suspects from a given library description file
    void load(
            QString libraryXML,        /// The xml document file for the library
            HistogramGrid gridTemplate /// A Histogram grid template to provide to the suspects
            );

    /// Fetches the suspects for the library from a given category
    QVector<SuspectPtr> suspects(
            int category = -1 /// The category of suspects to fetch.  -1 indicates all suspects
            );

    /// Fetches a mapping of the categories for the given library
    QMap< int, QString > categories();
};

}
