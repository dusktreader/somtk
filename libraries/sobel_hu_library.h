#pragma once

#include <QMap>
#include <QString>
#include <QVariant>
#include <QImage>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "histograms/histogram.h"
#include "libraries/image_library.h"
#include "suspects/grayscale_suspect.h"

namespace somtk {

class SobelHuLibrary : public ImageLibrary
{
public:
    SobelHuLibrary( HistogramGrid gridTemplate, QMap< QString, QVariant > libraryParameters );
    virtual ~SobelHuLibrary();

    virtual SuspectPtr generateSuspet( QImage rawImage );
};

} // namespace somtk
