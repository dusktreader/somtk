#pragma once
#include "feature.h"
#include "somhistogram.h"

#include "cv.h"
#include "cxcore.h"

#include "tools.hpp"
#include "cvtypesplus.hpp"

/** This abstract base class provides the basic functionality for a HSOM suspect */
class Suspect
{
private:
    int _predCat;
    int _realCat;
    SOMHistogram hist;
    std::string _name;
    cv::Mat_<double> _cats;

    Feature* currFeature;

public:

    /** Creates a new Suspect
      * @param  name    - The name of this Suspect
      * @param  realCat - The actual category for this suspect
      * @param  catCt   - The number of categories possible for this suspect
      * @param  sz      - The size of this Suspect's histogram
      */
    Suspect( std::string name, int realCat, int catCt, const SizePlus<int>& sz );

    /** Destructs this Suspect */
    virtual ~Suspect();

    /** Generates the next feature from this Suspect */
    virtual Feature* getNextFeature() = 0;

    /** Sets ANN input and ouput vectors based upon this suspect's histogram
      * @param  input   - The ANN's input vector
      * @param  output  - The ANN's output vector
      */
    void setANNVectors( cv::Mat_<double>& input, cv::Mat_<double>& output );

    /** Sets this Suspect's vote vector to an ANN's output
      * @param  output  - The ANN's output vector
      */
    void setCats( const cv::Mat_<double>& output );

    /** Fetches this Suspect's predicted category vector */
    cv::Mat<double> cats();

    /** Fetches the actual category of this Suspect */
    int realCat();

    /** Gets the predicted category of this Suspect */
    int predCat();

    /** Increments a bin in this Suspect's histogram at a specified location
      * @param  idx - The index of the bin to increment
      */
    void incrementHistogram( int idx );

    /** Increments a bin in this Suspect's histogram at a specified location
      * @param  pt - The location of the bin to increment
      */
    void incrementHistogram( const PointPlus<int>& pt );

    /** Normalizes this Suspect's histogram */
    void normalizeHistogram();

    /** Fetches a visualization of this histogram */
    cv::Mat vizHistogram();

    /** Fetches the name of this Suspect */
    std::string name();
};
