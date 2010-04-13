#pragma once

#include <exception>
#include <string>
#include <vector>

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include "tools.h"
#include "cvtypesplus.hpp"

class Detector{
private:
protected:

    bool isValid;
    std::string description;

    std::vector< std::pair<double,double> > plimits;

    cv::Mat flt;

    virtual bool read(  cv::FileNode& root ) = 0;
    virtual bool write( cv::FileStorage& fs ) = 0;

    void isolateMaxima( cv::Mat src, cv::Size sz, std::vector<cv::Rect>& detections );
    void isolateMaxima2( cv::Mat src, cv::Size detectionSize, std::vector<cv::Rect>& detections );

public:
    enum DType{ FASTHOUGH, TOPHAT, CASCADE, STAR };
    const std::string alias;
    const DType dtype;

    Detector( const std::string& alias, DType dtype );
    Detector( const std::string& fileName, const std::string& alias, DType dtype );
    virtual ~Detector();

    cv::Mat filtered();

    virtual bool load(  const std::string& fileName );
    virtual bool save(  const std::string& fileName );

    void checkParameters( const std::vector<double>& parameters );
    virtual void setParameters( const std::vector<double>& parameters ) = 0;
    virtual void getParameters( std::vector<double>& parameters ) const = 0;

    static std::string getAliasFromFile( const std::string& fileName );

    virtual void search( cv::Mat src, const cv::Size& minSize, const cv::Size& maxSize, std::vector<cv::Rect>& detections, bool procFlt=false ) = 0;
};

class DetectorException : public std::exception{
private:
    std::string message;
public:
    DetectorException( std::string message="Detector Error" ) : message(message){}
    ~DetectorException() throw(){}
    virtual const char* what() const throw(){ return message.c_str(); }

};

