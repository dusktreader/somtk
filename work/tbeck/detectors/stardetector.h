#pragma once

#include "cv.h"
#include "cxcore.h"
#include "detector.h"

class StarDetector : public Detector
{
private:
    /** Algorithm Parameters */
    int maxSizeIndex;
    int responseThreshold;
    int projectThreshold;
    int binaryThreshold;
    int suppression;

    cv::StarDetector* cvdetector;

    static const int maxSizes[];

    virtual void prepare();

protected:
    virtual bool read(  cv::FileNode& node );
    virtual bool write( cv::FileStorage& fs );

public:
    StarDetector();
    StarDetector( const std::string& fileName );
    virtual ~StarDetector();

    virtual void search( cv::Mat src, const cv::Size &minSize, const cv::Size &maxSize, std::vector<cv::Rect> &detections, bool procFlt );

    virtual void setParameters( const std::vector<double>& parameters );
    virtual void getParameters( std::vector<double>& parameters ) const;

    static const std::string uniqueAlias;
};
