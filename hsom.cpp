#include "hsom.h"

namespace somtk {

HSOM::HSOM( SOMPtr som, ClassifierPtr classifier ) :
    som( som ),
    classifier( classifier )
{}

HSOM::~HSOM(){}



QVector<FeaturePtr> HSOM::extractFeatures( QVector<SuspectPtr> suspects )
{
    /// @todo: Why is this being done by the hsom?  Move this to the library class.
    QVector<FeaturePtr> features;
    foreach( SuspectPtr suspect, suspects )
        features << suspect->features();
    return features;
}



void HSOM::generateHistograms( QVector<SuspectPtr> suspects )
{
    /// @todo Should this take a list of suspects or a library of them?
    for( int i=0; i<suspects.size(); i++ )
        generateHistogram( suspects[i] );
}



void HSOM::generateHistogram( SuspectPtr suspect )
{
    /** @todo Should this take a suspect or jsut a ist o freatures?
              Increment the suspect's histogram bin at the index of
              the closest feature for each suspect
     **/
    foreach( FeaturePtr feature, suspect->features() )
        suspect->histogram()->increment( som->closestFeature( feature ) );
}



void HSOM::train( QVector<SuspectPtr> trainingSuspects,
                  QMap<QString, QVariant> somParameters,
                  QMap<QString, QVariant> classifierParameters )
{
    /// @todo: This should probably take a library not a list of suspects
    //try
    //{
        QVector<FeaturePtr> features = extractFeatures( trainingSuspects );
        som->train( somParameters, features );
        generateHistograms( trainingSuspects );
        classifier->train( trainingSuspects, classifierParameters );
    //}
    //catch( SOMError err )
    //{
    //    throw err;
    //}
    //catch( ... )
    //{
    //    throw;
    //}
}



void HSOM::classify( QVector<SuspectPtr> suspects )
{
    /// @todo: Is this really necessary?  Do I really want to make a function that only wraps a for loop?
    foreach( SuspectPtr suspect, suspects )
        classify( suspect );
}


void HSOM::classify( SuspectPtr suspect )
{
    generateHistogram( suspect );
    classifier->classify( suspect );
}

} // namespace hsom
