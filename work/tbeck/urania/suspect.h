#pragma once
#include "feature.h"
#include "somhistogram.h"
#include "local.h"

class Suspect{
private:
    int predCat;
    int realCat;
    SOMHistogram* hist;
    std::string name;
    std::vector<double> votes;
    int catCt;

public:

    /** Creates a new Suspect
      * @param  name    - The name of the suspect
      * @param  realCat - The category for this suspect
      * @param  predCat - The predicted category for this suspect
      * @param  w       - The correct width of the suspect's histogram
      * @param  h       - The correct height of the suspect's histogram
      */
    Suspect( std::string name, int realCat, int catCt, int w, int h );

    /** Destructs the Suspect */
    virtual ~Suspect();

    /** Fetches the next feature out of the suspect's data */
    virtual Feature* getNextFeature() = 0;

    /** Sets ANN input and ouput vectors based upon the suspect's histogram
      * @param  input   - The ANN's input vector
      * @param  inputW  - The width of the ANN's input vector
      * @param  output  - The ANN's output vector
      * @param  outputW - The width of the ANN's output vector
      */
    void setANNVectors( double* input, int inputW, double* output=NULL, int outputW=0 );

    /** Set's the Suspect's vote vector to an ANN's output
      * @param  output  - The ANN's output vector
      * @param  outputW - The width of the ANN's output vector
      */
    void setCatVotes( double* output, int outputW );

    /** Fetches the Suspect's vote vector */
    std::vector<double> getCatVotes();

    /** Fetches the actual category of this Suspect
      * @return The suspect's actual category that was set at initialization
      */
    int getRealCat();

    /** Gets the predicted category of this Suspect */
    int getPredCat();

    /** Increments a bin in the Suspect's histogram at a specified location
      * @param  index - The index of the bin to increment
      */
    void incrementHistogram( int index );

    /** Increments a bin in the Suspect's histogram at a specified location
      * @param  x - The x location of the bin
      * @param  y - The y location of the bin
      */
    void incrementHistogram( int x, int y );

    /** Normalizes the Suspect's histogram */
    void normalizeHistogram();

    /** Shows a visualization of the histogram */
    void showHistogram( std::string msg="" );

    /** Fetches the name of the Suspect */
    std::string getName();
};
