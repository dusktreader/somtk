#pragma once

#include "suspect.h"
#include "feature.h"
#include "hufeature.h"

#include "cv.h"
#include "cxcore.h"

#include "tools.hpp"
#include "cvtools.hpp"
#include "cvtypesplus.hpp"

/// @todo Make step size a parameter of ImageSuspect's constructor
#define STEP_SIZE 2

class ImageSuspect : public Suspect
{
private:
    cv::Mat img;
    cv::Mat msk;
    RectPlus<int> roi;
    PointPlus<int> imgPt;
    SizePlus<int> featSz;
    HuFeature currFeat;

public:

    /** Creates a new ImageSuspect
      * @param  img     - The image that this Suspect represents
      * @param  msk     - A masked version of the image
      * @param  realCat - The actual category for this Suspect
      * @param  catCt   - The number of categories possilbel for this Suspect
      * @param  sz      - The size of this Suspect's histogram
      * @param  featSz  - The size of features this Suspect generates
      * @param  name    - The name of this Suspect
      */
    ImageSuspect( const cv::Mat& img, const cv::Mat& msk, int realCat, int catCt, const SizePlus<int>& sz, const SizePlus<int>& featSz, std::string name );

    /** Destructs this suspect */
    virtual ~ImageSuspect();

    /** Resets the size of the features this Suspect generates */
    void resetFeatureSz( const SizePlus<int>& featSz );

    /** Generates the next feature from this Suspect */
    virtual void getNextFeature( Feature& feat );
};
