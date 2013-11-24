#pragma once

#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include <opencv2/highgui/highgui.hpp>

#include "libraries/suspect_library.h"
#include "errors/somerror.h"
#include "suspects/imagesuspect.h"
#include "histograms/histogram.h"

namespace somtk {
class ImageLibrary : public SuspectLibrary
{
protected:

    /// Generates an ImageSuspect given a raw opencv image
    virtual ImageSuspectPtr generateSuspect(
            cv::Mat rawImage
            ) = 0;



public:
    ImageLibrary( HistogramGrid gridTemplate, QMap< QString, QVariant > libraryParameters );
    virtual ~ImageLibrary();

    virtual SuspectPtr loadSuspect(
            QDomElement suspectElement
            );
};

} // namespace somtk
