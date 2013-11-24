#include "hsom.h"

namespace somtk {

HSOM::HSOM( SOMPtr som, ClassifierPtr classifier ) :
    som( som ),
    classifier( classifier )
{}

HSOM::~HSOM(){}



void HSOM::generateHistogram( SuspectPtr suspect )
{
    foreach( FeaturePtr feature, suspect->features() )
        suspect->histogram()->increment( som->closestFeature( feature ) );
}



void HSOM::train( SuspectLibraryPtr suspectLibrary,
                  QMap<QString, QVariant> somParameters,
                  QMap<QString, QVariant> classifierParameters )
{
    /// @todo: This should probably take a library not a list of suspects
    //try
    //{
        QVector< FeaturePtr > features = suspectLibrary->extractFeatures();
        som->train( somParameters, features );
        foreach( SuspectPtr suspect, suspectLibrary->suspects() )
        {
            generateHistogram( suspect );
        }
        classifier->train( suspectLibrary, classifierParameters );
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

void HSOM::classify( SuspectPtr suspect )
{
    generateHistogram( suspect );
    classifier->classify( suspect );
}

} // namespace hsom
