#pragma once

#include "suspect.h"
#include "feature.h"
#include "hufeature.h"

#include "cv.h"
#include "cxcore.h"

#include "tools.hpp"
#include "cvtools.hpp"
#include "cvtypesplus.hpp"

class ImageSuspect : public Suspect
{
private:
    cv::Mat img;
    RectPlus<int> roi;
    PointPlus<int> imgPt;
    SizePlus<int> featSz;
    int stepSz;

public:

    /** Creates a new ImageSuspect
      * @param  img     - The image that this Suspect represents
      * @param  realCat - The actual category for this Suspect
      * @param  catCt   - The number of categories possilbel for this Suspect
      * @param  histSz  - The size of this Suspect's histogram
      * @param  featSz  - The size of features this Suspect generates
      * @param  stepSz  - The size of steps this Suspect takes as it traverses an image
      * @param  name    - The name of this Suspect
      */
    ImageSuspect( const cv::Mat& img, int realCat, int catCt, const SizePlus<int>& histSz, const SizePlus<int>& featSz, int stepSz, std::string name );

    /** Destructs this suspect */
    virtual ~ImageSuspect();

    /** Resets the size of the features this Suspect generates */
    void resetFeatureSz( const SizePlus<int>& featSz );

    /** Generates the next feature from this Suspect */
    virtual Feature* getNextFeature();
};
