#pragma once

#include <string>

#include "cv.h"
#include "cxcore.h"
#include "ml.h"

#include "detector.h"

class CascadeDetector : public Detector, public cv::CascadeClassifier{
private:
    /** Algorithm Parameters */
    double scaleFactor;
    int    minNeighbors;

    virtual void prepare();

protected:
    virtual bool read(  cv::FileNode& node );
    virtual bool write( cv::FileStorage& fs );

public:
    CascadeDetector();
    CascadeDetector( const std::string& fileName );
    virtual ~CascadeDetector();

    virtual bool load( const std::string& fileName );

    virtual void search( cv::Mat src, const cv::Size &minSize, const cv::Size &maxSize, std::vector<cv::Rect> &detections, bool procFlt );

    virtual void setParameters( const std::vector<double>& parameters );
    virtual void getParameters( std::vector<double>& parameters ) const;

    static const std::string uniqueAlias;
};

