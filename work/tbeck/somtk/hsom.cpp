#include "hsom.h"

namespace somtk {

HSOM::HSOM( SOMPtr som, ClassifierPtr classifier ) :
    som( som ),
    classifier( classifier )
{}



QVector<FeaturePtr> HSOM::extractFeatures( QVector<SuspectPtr> suspects )
{
    QVector<FeaturePtr> features;
    foreach( SuspectPtr suspect, suspects )
        features << suspect->features();
    return features;
}



void HSOM::generateHistograms( QVector<SuspectPtr> suspects )
{
    for( int i=0; i<suspects.size(); i++ )
        generateHistogram( suspects[i] );
}



void HSOM::generateHistogram( SuspectPtr suspect )
{
    // Increment the suspect's histogram bin at the index of the closest feature for each suspect
    foreach( FeaturePtr feature, suspect->features() )
        suspect->histogram()->increment( som->closestFeature( feature ) );
}



void HSOM::train( QVector<SuspectPtr> trainingSuspects,
                  QMap<QString, QVariant> somParameters,
                  QMap<QString, QVariant> classifierParameters )
{
    try
    {
        QVector<FeaturePtr> features = extractFeatures( trainingSuspects );
        som->train( somParameters, features );
        generateHistograms( trainingSuspects );
        classifier->train( trainingSuspects, classifierParameters );
    }
    catch( SOMError err )
    {
        throw err;
    }
    catch( ... )
    {
        throw;
    }
}



void HSOM::classify( QVector<SuspectPtr> suspects )
{
    foreach( SuspectPtr suspect, suspects )
        classify( suspect );
}


void HSOM::classify( SuspectPtr suspect )
{
    generateHistogram( suspect );
    classifier->classify( suspect );
}

} // namespace hsom
