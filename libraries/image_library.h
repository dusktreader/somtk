#pragma once

#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "errors/somerror.h"
#include "histograms/histogram.h"

namespace somtk {
class ImageLibrary : public SuspectLibrary
{
public:
    ImageLibrary( HistogramGrid gridTemplate, QMap< QString, QVariant > libraryParameters );
    virtual ~ImageLibrary();

    virtual void loadSuspect(
            QDomElement suspectElement
            );

    virtual SuspectPtr generateSuspect(
            QImage rawImage
            ) = 0;
};

} // namespace somtk
