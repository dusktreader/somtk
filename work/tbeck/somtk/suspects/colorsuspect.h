#pragma once

#include "suspect.h"
#include "feature.h"
#include "colorfeature.h"

#include "cv.h"
#include "cxcore.h"

#include "tools.hpp"
#include "cvtools.hpp"
#include "cvtypesplus.hpp"

class ColorSuspect : public Suspect
{
private:
    cv::Mat img;
    std::vector< cv::Mat_<uchar> > spectrum;
    PointPlus<int> imgPt;
public:
    ColorSuspect();

    /** Creates a new ColorSuspect
      * @param  img     - The image that this Suspect represents
      * @param  realCat - The actual category for this Suspect
      * @param  catCt   - The number of categories possilbel for this Suspect
      * @param  gridSz  - The size of this Suspect's histogram
      * @param  name    - The name of this Suspect
      */
    ColorSuspect( const cv::Mat& img, int realCat, int catCt, const SizePlus<int>& histSz, std::string name );

    /** Destructs this suspect */
    virtual ~ColorSuspect();

    /** Generates the next feature from this Suspect */
    virtual Feature* getNextFeature();
};
