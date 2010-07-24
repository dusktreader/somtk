#pragma once

#include "cv.h"
#include "cxcore.h"
#include "ml.h"

#include "tools.hpp"
#include "cvtypesplus.hpp"

#include "hexgrid.hpp"
#include "suspect.h"

#include <list>

/** A slope tuning parameter for an inverse exponential function */
#define A 0.1

/** Adjustment parameter for the initial width of the neighborhood function */
#define B 0.20

/** The SOM class provides an abstract base class for Self-Organizing Maps */
class HSOM
{
private:

    /** The alias for this class */
    static const std::string alias;

    /** The name of this HSOM */
    std::string name;

    /** The back-end Multilayer Perceptron Neural Network for classification */
    CvANN_MLP* ann;

    HexGrid<Feature*> grid;

    /** Trains the SOM component of the HSOM
      * @param  somEpochs  - The number of training epochs to use
      * @param  initAlpha  - The initial training weight
      * @param  initR      - The intial radius of the training neighborhood
      */
    void trainSOM( int somEpochs, double initAlpha, double initR );

    /** Generates the SOMHistograms needed for trainning the HSOM
      * @return A boolean indicating if the training was interrupted
      */
    void generateHistograms();

    /** Trains the ANN component of the HSOM
      * @param  annIters   - The number of iterations for ANN training
      * @param  annEps     - The minimum adjustment size for ANN training
      * @return A boolean indicating if the training was interrupted
      */
    void trainANN( int annIters, double annEps );

protected:

    /** A list of suspects for training or classification */
    std::list<Suspect*> suspects;

    /** A set of precalculated gaussian weights for updating the SOM */
    std::vector<double> weights;

    /** The number of categories that this Hybrid SOM will classifiy */
    int _catCt;

    /** Performs a status check that may interface to an interactive control
      * @param  iteration - The current iteration
      * @param  msg1      - An optional message
      * @param  msg2      - Another optional message
      * @param  maxIter   - The maximum number of iterations to perform
      * @return A boolean indicating if processing should continue
      */
    virtual bool statusCheck( int iteration, std::string msg1="", std::string msg2="", int maxIters=0 );

    /** Preprocessing for the suspects.  Implemented by child class. */
    virtual bool analyzeSuspects( ) = 0;

    /** Initializes the features of this SOM */
    virtual void initFeatures() = 0;

    /** Finds the feature that is closest to the input feature
      * @param  feat - The input feature to compare against map features
      * @return The index of the closest feature
      */
    int closestFeature( Feature* feat );

    /** Finds the coordinates of the closest feature to the input feature
      * @param  feat - The input feature to compare against map features
      * @param  x    - The 'x' coordinate of the closest feature
      * @param  y    - The 'x' coordinate of the closest feature
      */
    void closestFeature( Feature* feat, int &x, int &y );

    /** Precalculates Gaussian wieghts given a radius and an alpha
      * @param  alpha - The learning rate
      * @param  radius - The width of the Gaussian weighting function
      */
    void preCalcWeights( const double alpha, const double radius );

    /** Updates the SOM with a single training suspect
      * @param  suspect - The suspect with which to train
      */
    void updateSOM( Suspect* suspect );

    /** Updates the training suspects histogram given a trained SOM
      * @param  suspect - The suspect to update
      */
    void updateHistogram( Suspect* suspect );

public:

    /** Constructs an empty HSOM */
    HSOM();

    /** Constructs the Hybrid SOM with a specified size
      * @param  sz    - The desired size of the SOM
      * @param  catCt - Then number of categories that this Hybrid SOM can classify
      */
    HSOM( const SizePlus<int>& sz, int catCt );

    /** Destructs the SOM */
    virtual ~HSOM();

    /** Clears the SOM */
    virtual void clear();

    /** Loads the suspects from a directory.  Implemented by derived class
      * @param  dirPath - The directory holding the image files
      * @param  fileList - The list of image files
      * @return A flag indicating if loading was successful
      */
    virtual bool loadSuspects( std::string dirPath, const std::vector<std::string> &fileList ) = 0;

    /** Clears the suspect list */
    void clearSuspects();

    /** Trains the Hybrid SOM
      * @param  somEpochs  - The number of training epochs to use
      * @param  initAlpha  - The initial training weight
      * @param  initR      - The intial radius of the training neighborhood
      * @param  annIters   - The number of iterations for ANN training
      * @param  annEps     - The minimum adjustment size for ANN training
      */
    void train( int somEpochs, double initAlpha, double initR, int annIters, double annEps );

    /** Report classification results
      * @param  text - The string that will contain the report
      */
    double report( std::string &report );

    /** Classifies suspects
      * @param  A flag indicating if the function finished or was terminated
      */
    void classify();

    /** Classifies a single suspect
      * @param  suspect - The suspect to classify
      * @param  input   - The input matrix for the ANN
      * @param  output  - The output matrix for the ANN
      */
    void classify( Suspect* suspect, cv::Mat& input, cv::Mat& output );

    /** Fetches the name of this HSOM */
    std::string getName();

    /** Gets the number of categories this HSOM is trained for */
    int getCatCt();

    /** Saves an SOM to file
      * @param  fileName - The name of the file to save
      */
    void save( std::string fileName );

    /** Loads an SOM from file
      * @param  fileName - The name of the file to load
      */
    void load( std::string fileName );

    /** Reads ImageHSOM configuration from a file storage object
    * @param  fs   - The file node from which to read
    */
    virtual void read( cv::FileNode& fn );

    /** Writes the ImageHSOM configuration to a file storage object
    * @param  fs - The file storage in which to write
    */
    virtual void write( cv::FileStorage& fs );

};
