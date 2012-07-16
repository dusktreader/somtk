#include "hsom.h"

namespace somtk {

HSOM::HSOM( SOMPtr som, ClassifierPtr classifier ) :
    som( som ),
    classifier( classifier )
{}



QVector<Feature> HSOM::extractFeatures( QVector<SuspectPtr> suspects )
{
    QVector<Feature> features;
    foreach( SuspectPtr suspect, suspects )
        foreach( Feature feature, suspect->features() )
            features.append( feature );
    return features;
}



void HSOM::generateHistograms( QVector<SuspectPtr> suspects )
{
    for( int i=0; i<suspects.size(); i++ )
        generateHistogram( trainingSuspects[i] );
}



void HSOM::generateHistogram( SuspectPtr suspect )
{
    // Increment the suspect's histogram bin at the index of the closest feature for each suspect
    foreach( Feature feature, suspect->features() )
        suspect->histogram()->increment( som->closestFeature( feature ) );
}



void HSOM::train( QVector<SuspectPtr>& suspects,
                  QMap<QString, QVariant> somParameters,
                  QMap<QString, QVariant> classifierParameters )
{
    try
    {
        QVector<Feature> features = extractFeatures( suspects );
        som->train( somParameters, features );
        generateHistograms( suspects );
        classifier->train( suspects, classifierParameters );
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
