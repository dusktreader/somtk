#pragma once
#include "hsom.h"
#include "feature.h"
#include "suspect.h"
#include "imagesuspect.h"
#include "local.h"
#include "ThumbData.h"
#include "SubClassifier.h"

#define IMAGE_HSOM_ALIAS "IMAGE_HSOM"

/** The ImageHSOM class provides an image based Self-Organizing Map that uses distinct image features to classify */
class ImageHSOM : public HSOM, public SubClassifier{
private:

    /** The width of features in this HSOM */
    int featW;

    /** The height of features in this HSOM */
    int featH;

public:

    /** Constructs an empty ImageHSOM */
    ImageHSOM();

    /** Constructs an Image HSOM with specified size, feature size, and wrapping
      * @param  w     - The desired width of the HSOM
      * @param  h     - The desired height of the HSOM
      * @param  featW - The desired width of the features in the HSOM
      * @param  featH - The desired height of the features in the HSOM
      * @param  catCt - The number of possible suspect classes
      */
    ImageHSOM( int w, int h, int featW, int featH, int catCt );

    /** Constructs an Image HSOM from a save file
      * @param  fileName - The name of the file from which to load the HSOM
      */
    ImageHSOM( string fileName );

    /** Constructs an Image HSOM from a file storage
      * @param  fs   - The file storage from which to read
      * @param  node - The node in which to begin reading
      */
    ImageHSOM( CvFileStorage* fs, CvFileNode* node );

    /** Destructs the ImageHSOM */
    virtual ~ImageHSOM();

    /** Fetches the size of features for this ImageHSOM
      * @param  featW - The width of the feature
      * @param  featH - The height of the feature
      */
    void getFeatSize( int &featW, int &featH );

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
