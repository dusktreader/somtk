#include "hsom.h"

namespace hsom {

HSOM::HSOM( QObject* parent ) :
    QThread( parent )
{}



HSOM::HSOM( NormalizerPtr normalizer, SOMPtr som, ClassifierPtr classifier, QObject* parent ) :
    QThread( parent ),
    normalizer( normalizer ),
    som( som ),
    classifier( classifier )
{}



void HSOM::clear()
{
    normalizer->clear();
    som->clear();
    classifier->clear();
}



QVector<Feature> HSOM::extractFeatures( QVector<SuspectPtr> suspects )
{
    QVector<Feature> features;
    foreach( SuspectPtr suspect, suspects )
        foreach( Feature feature, suspect->features() )
            features.append( feature );
    return features;
}



void HSOM::generateHistograms( QVector<SuspectPtr>& suspects )
{
    for( int i=0; i<suspects.size(); i++ )
        generateHistogram( trainingSuspects[i] );
}



void HSOM::generateHistogram( SuspectPtr suspect )
{
    foreach( Feature feature, suspect->features() )
    {
        // Find the feature that is closest to the input feature
        QPoint pt = som->closestFeatureCoords( feature );

        // Increment the suspect's histogram bin at the point of the closest feature
        suspect->histogram()->increment( pt );
    }
}



void HSOM::train( QVector<SuspectPtr>& suspects,
                  QMap<QString, QVariant> normalizerParameters,
                  QMap<QString, QVariant> somParameters,
                  QMap<QString, QVariant> classifierParameters )
{
    try
    {
        QVector<Feature> features = extractFeatures( suspects );
        normalizer->calculateNormalizer( features, normalizerParameters );
        foreach( Feature feature, features )
            normalizer->normalize( feature );
        som->train( features, normalizer, somParameters );
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


void HSOM::classify( SuspectPtr suspect )
{
    QVector<Feature> features = suspect->features();
    normalizeFeatures( features );

    generateHistogram( suspect );

    classifier->classify( suspect );
}

} // namespace hsom
