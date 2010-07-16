#pragma once
#include "feature.h"
#define HU7_FIX

/** The HuFeature class provides a feature based upon image data */
class HuFeature : public Feature
{
private:

    static const std::string alias;
    std::vector<double> huVals;

    static std::vector<double> huAlpha;                                                                                                                        // Generated with training set and epsilon = 0.625
    static std::vector<double> huMean;
    static std::vector<double> huStdv;

public:

    /** Default Constructor */
    HuFeature();

    /** Constructs a feature image with specified size and source image
      * @param  img - The image source for the data
      * @note   The image may be color or grayscale and any size
      */
    HuFeature( const cv::Mat& img );

    virtual ~HuFeature();

    /** Get the original Hu moments calculated for this feature */
    std::vector<double> getOriginalHus();

    /** Sets the parameters for the sigmoid correction function
      @param  huMean  - The mean values for all hu features
      @param  huStdv  - The standard deviations for all hu features
      @param  huAlpha - The alpha tunting parameter for all hu features
      */
    static void setSigmoidParams( const std::vector<double>& huMean, const std::vector<double>& huStdv, const std::vector<double>& huAlpha );

    /** Fetches the parameters for the sigmoid correction function
      @param  huMean  - The mean values for all hu features
      @param  huStdv  - The standard deviations for all hu features
      @param  huAlpha - The alpha tunting parameter for all hu features
      */
    static void getSigmoidParams( std::vector<double>& huMean, std::vector<double>& huStdv, std::vector<double>& huAlpha );

    /** This sigmoid squashing function maps hu values to the range ( 0, 1 )
      * @param  t   - The input hu value
      * @param  idx - The index of the hu value
      */
    double sigmoid( double t, int idx );

};
