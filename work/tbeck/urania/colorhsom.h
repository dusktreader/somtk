#pragma once

#include "cv.h"
#include "cxcore.h"

#include "hsom.h"
#include "feature.h"
#include "colorfeature.h"
#include "colorsuspect.h"

class ColorHSOM : public HSOM
{
private:
    static const std::string alias;

public:
    ColorHSOM();
    ColorHSOM( const SizePlus<int>& gridSz, int catCt );
    ColorHSOM( const std::string& fileName );
    ColorHSOM( cv::FileNode& fn );
    virtual ~ColorHSOM();
    virtual void initFeatures();

    virtual void loadSuspects( const std::string& dirPath, const std::vector<std::string>& fileList );
    virtual void analyzeSuspects();

    cv::Mat vizHSOM();

    virtual void read(  cv::FileNode&    fn);
    virtual void write( cv::FileStorage& fs);
};
