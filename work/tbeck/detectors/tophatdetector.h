#pragma once

#include "detector.h"

class TophatDetector : public Detector{
private:
    /** Algorithm Parameters */
    int crownRadius;
    int brimWidth;
    double hatHeight;
    int stepSize;

    cv::Mat crownMask;
    cv::Mat brimMask;

    uchar calcHat( const cv::Mat& src, const cv::Rect& roi );
    void prepare();

protected:

    virtual bool read(  cv::FileNode& root );
    virtual bool write( cv::FileStorage& fs );

public:
    TophatDetector();
    TophatDetector( const std::string& fileName );
    virtual ~TophatDetector();

    virtual void search( cv::Mat src, const cv::Size& minSize, const cv::Size& maxSize, std::vector<cv::Rect> &detections, bool procFlt=false );

    virtual void setParameters( const std::vector<double>& parameters );
    virtual void getParameters( std::vector<double>& parameters ) const;

    static const std::string uniqueAlias;
};
