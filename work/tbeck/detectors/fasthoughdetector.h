#pragma once

#include "detector.h"

#define ZERO cv::Scalar( 0.0 )

class FastHoughDetector : public Detector{
private:
    /** Algorithm Parameters */
    int    windowWidth;
    double windowStepScale;
    double detectionInfluence;
    int    detectionThreshold;

    /** Fixed Parameters */
    int    thresholdWidth;
    int    thresholdOffset;
    double thetaResolution;

    int rCt;
    int tCt;
    cv::Mat crc;
    int crcOff;
    std::vector< std::vector< std::vector<int> > > rhoTable;
    void initRhoTable();
    bool fastHough( const cv::Mat& src, cv::Mat& acc ) const;
    void prepare();


protected:
    virtual bool read(  cv::FileNode& root );
    virtual bool write( cv::FileStorage& fs );

public:
    FastHoughDetector();
    FastHoughDetector( const std::string& fileName );
    virtual ~FastHoughDetector();

    virtual void search( cv::Mat src, const cv::Size& minSize, const cv::Size& maxSize, std::vector<cv::Rect> &detections, bool procFlt=false );

    virtual void setParameters( const std::vector<double>& parameters );
    virtual void getParameters( std::vector<double>& parameters ) const;

    static const std::string uniqueAlias;
};
