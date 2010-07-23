#pragma once
#include "hsom.h"
#include "feature.h"
#include "suspect.h"
#include "imagesuspect.h"

#include "tools.hpp"
#include "cvtypesplus.hpp"

#include "cv.h"
#include "cxcore.h"

/** The ImageHSOM class provides an image based Self-Organizing Map that uses distinct image features to classify */
class ImageHSOM : public HSOM
{
private:

    /** The size of features in this HSOM */
    SizePlus<int> _featSz;

public:

    /** Constructs an empty ImageHSOM */
    ImageHSOM();

    /** Constructs an Image HSOM with specified size, feature size, and wrapping
      * @param  sz     - The desired size of the HSOM's grid
      * @param  featSz - The desired size of the features in the HSOM
      * @param  catCt - The number of possible suspect classes
      */
    ImageHSOM( const SizePlus<int>& sz, const SizePlus<int>& featSz, int catCt );

    /** Constructs an Image HSOM from a save file
      * @param  fileName - The name of the file from which to load the HSOM
      */
    ImageHSOM( string fileName );

    /** Constructs an Image HSOM from a file node
      * @param  fn   - The file node from which to read
      */
    ImageHSOM( cv::FileNode& fn );

    /** Destructs the ImageHSOM */
    virtual ~ImageHSOM();

    /** Initializes the features in the HSOM */
    virtual void initFeatures();

    /** Fetches the size of features for this ImageHSOM */
    void featSz();

    /** Loads a list of suspects from a directory
      * @param  dirPath - The directory holding the image files
      * @param  fileList - The list of image files
      * @return A flag indicating if loading was successful
      */
    virtual bool loadSuspects( std::string dirPath, const std::vector<std::string> &fileList );

    /** Analyzes the hu moments of features from the suspect list to calibrate the sigmoid function */
    virtual void analyzeSuspects();

    /** Reads ImageHSOM configuration from a file storage object
      * @param  fs   - The file node from which to read
      */
    virtual void read( const cv::FileNode& fn );

    /** Writes the ImageHSOM configuration to a file storage object
      * @param  fs - The file storage in which to write
      */
    virtual void write( cv::FileStorage& fs );

};
