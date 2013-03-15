#ifndef IMAGESUSPECT_H
#define IMAGESUSPECT_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "suspects/suspect.h"

namespace somtk {
class ImageSuspect : public Suspect
{

protected:

    QImage _originalImage;

    virtual void generateFeatures();

    double _contentThreshold;

    /// Filters an input image to prepare it for feature extraction
    virtual void filterImage();

    /// Indicaes whether an image window has any content from which to generate a feature
    virtual bool hasContent(
            QRect window /// The image window in which to check for content
            ) = 0;

    /// Extracts a feature from the given window
    virtual FeaturePtr extractFeature(
            QRect window /// The image window from which to extract a feature
            ) = 0;



public:

    ImageSuspect( QImage image, HistogramGrid gridTemplate );
};

} // namespace somtk

#endif // IMAGESUSPECT_H
