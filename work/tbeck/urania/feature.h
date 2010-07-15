#pragma once
#include "local.h"

#define FEATURE_ALIAS "FEATURE"

/** The feature class provides an abstract base class for SOM features */
class Feature
{
protected:

    /** The data associated with the feature */
    std::vector<double> data;

    /** Sets the size of the data vector
      * @param  l - The length of the vector
      */
    void setSize( int l );

    /** Sets the data to a given input vector
      * @param  newData - The new data
      */
    void setData( const std::vector<double> &newData );

public:

    virtual ~Feature();

    /** Calculates the distance between this and another feature
      * @param  other - The other feature to compare against
      * @return The distance between the two features
      */
    virtual double dist( const Feature& other );

    /** Adjusts the value of this Feature given another and other parameters
      * @param  other       - The other Feature to compare against
      * @param  scaleFactor - A scaling factor for adjustment
      */
    virtual void adjust( const Feature& other, double scaleFactor );

    int l();

    void read( CvFileStorage* fs, CvFileNode* node=NULL );
    void write( CvFileStorage* fs );

};
