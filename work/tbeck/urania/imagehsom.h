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

    /** Fetches the size of features for this ImageHSOM */
    void featSz();

    /** Loads a list of suspects from a directory
      * @param  dirPath - The directory holding the image files
      * @param  fileList - The list of image files
      * @return A flag indicating if loading was successful
      */
    virtual bool loadSuspects( std::string dirPath, const std::vector<std::string> &fileList );

    /** Creates an ImageSuspect from an image, category, and name
      * @param  img     - The picture of the suspect
      * @param  msk     - The mask image of the suspect ( if provided )
      * @param  realCat - The category of the suspect
      * @param  name    - The name of the suspect
      * @return The new suspect
      */
    ImageSuspect* createSuspect( IplImage* img, IplImage* msk=NULL, int realCat=0, std::string name="unnamed" );

    /** Analyzes the hu moments of features from the suspect list to calibrate the sigmoid function */
    virtual bool analyzeSuspects();

    /** Initializes the features in the HSOM */
    virtual void initFeatures();

    /** Reads ImageHSOM configuration from a file storage object
      * @param  fs   - The file storage from which to read
      * @param  node - The node at which to begin looking.  If NULL, start from the root
      */
    virtual void read(  CvFileStorage* fs, CvFileNode* node=NULL );

    /** Writes the ImageHSOM configuration to a file storage object
      * @param  fs - The file storage in which to write
      */
    virtual void write( CvFileStorage* fs );

    /* The following functions add compatability with SubClassifier framework */

    virtual void AddData( ThumbData &input );
    virtual void Train();
    virtual void Read( CvFileStorage* fs, CvFileNode* fsNode );
    virtual void Write( CvFileStorage* fs );
    std::vector<int> PredictCategorical( ThumbData &input );

};
