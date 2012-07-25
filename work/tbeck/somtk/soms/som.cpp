#include "som.h"

namespace somtk {

SOM::SOM( FeatureGrid grid, NormalizerPtr normalizer ) : _grid( grid ), _normalizer( normalizer ){}

SOM::~SOM(){}

void SOM::initializeTraining( QMap<QString, QVariant> somParameters, QVector<FeaturePtr> features )
{
    SOMError::requireCondition( features.count() > 0, "Cannot train with an empty feature set" );

    int featureSize = features.front()->size();
    SOMError::requireCondition( featureSize > 0 , "Feature size must be greater than 0" );

    bool ok = true;

    // Describes the number of epochs of training the SOM will need
    SOMError::requireCondition( somParameters.contains( "maxEpochs" ), "SOM parameters doesn't contain epoch count" );
    maxEpochs = somParameters["maxEpochs"].toInt( &ok );
    SOMError::requireCondition( ok, "Couldn't convert maximum epoch count to an int" );

    // Describes the inital training weight for the SOM
    SOMError::requireCondition(
                somParameters.contains( "initialAlpha" ),
                "SOM parameters doesn't contain initial alpha"
                );
    initialAlpha = somParameters["initialAlpha"].toDouble( &ok );
    /// @todo  Add range check to alpha (hint on values)
    /// @todo  Perhaps use default values if the parameters aren't specified
    SOMError::requireCondition( ok, "Couldn't convert initial alpha to a double" );

    // Describes the inital radius of the training neighborhood for the SOM
    SOMError::requireCondition(
                somParameters.contains( "initialRadiusRatio" ),
                "SOM parameters doesn't contain initial radius ratio"
                );
    double initialRadiusRatio = somParameters["initialRadiusRatio"].toDouble( &ok );
    SOMError::requireCondition( ok, "Couldn't convvert initial radius ratio to a double" );
    SOMError::requireCondition( initialRadiusRatio < 0.5, "Initial radius ratio may not exceed 0.5" );
    SOMError::requireCondition( initialRadiusRatio > 0.0, "Initial radius ratio must be greater than 0" );


    /// @todo  determine if there should be other constraints to alpha and radius ratio (negatives, ffs!)

    alpha_tf = 0.10;   // This is a tuning factor...should not be hardcoded
    alpha_Nf = 0.25;   // This is a tuning factor...should not be hardcoded
    alpha_gamma = -log( alpha_Nf ) / ( alpha_tf * maxEpochs );

    initialRadius = initialRadiusRatio * _grid->diagonal();
    radius_tf = 0.25;   // This is a tuning factor...should not be hardcoded
    radius_Nf = 0.50;   // This is a tuning factor...should not be hardcoded
    radius_gamma = -log( radius_Nf ) / ( radius_tf * maxEpochs );

    currentEpoch = -1;
    nextEpoch();

    // Calculate the normalizer given the training features
    _normalizer->calculateNormalizer( features );

    for( int i=0; i<_grid->capacity(); i++ )
    {
        FeaturePtr newFeature( new Feature( featureSize ) );
        _normalizer->setFeature( newFeature );
        _grid->item(i) = newFeature;
    }

    // Normalize the input features
    _normalizer->normalizeAll( features );
}



void SOM::train( QMap<QString, QVariant> somParameters, QVector<FeaturePtr> features, bool skipInit_debugOnly )
{

    if( skipInit_debugOnly == false )
        initializeTraining( somParameters, features );

    do
    {
       // Shuffle the list of features to remove bias
        std::random_shuffle( features.begin(), features.end() );

        // Update the SOM with each feature from the globalFeatures list.
        // @todo: consider random sampling for the training if the number of features is high
        foreach( FeaturePtr feature, features )
            update( feature );

    } while( nextEpoch() != false );

}



bool SOM::nextEpoch()
{
    // Increase the current epoch by one
    currentEpoch++;

    // If the current epoch is equal to the maximum number of epochs indicate that training is finished
    if( currentEpoch >= maxEpochs )
        return false;

    // Calculate the current alpha and radius using the exponential decay functions
    currentAlpha = initialAlpha * exp( -alpha_gamma * currentEpoch );
    currentRadius = initialRadius * exp( -radius_gamma * currentEpoch );

    // Precalculate the weights for updating features at fixed distances
    precalculateWeights();

    return true;
}



void SOM::precalculateWeights()
{
    /* The radius is used to describe the Full-width at Half-maximum of the gaussian weighting function
     * Sigma is back-computed from this
     */
    double sigma = currentRadius / FWHM_FACTOR;

    double twoSigmaSquared = 2 * pow( sigma, 2.0 );

    weights = QVector<double>( (int)currentRadius + 1, 0.0 );

    /* Calculate the gaussian weighting function.
     * The function is dependent on alpha, sigma, and the current distance from the center of the neighborhood
     */
    for( int i = 0; i < weights.size(); i++ )
        weights[i] = currentAlpha * exp( -pow( i, 2.0 ) / twoSigmaSquared );
}



int SOM::closestFeature( FeaturePtr feature )
{
    if( !feature->isNormalized() )
        _normalizer->normalizeFeature( feature );

    double globalMinimumDistance = DBL_MAX;
    int    globalMinimumIndex    = -1;

    double localMinimumDistance = DBL_MAX;
    int    localMinimumIndex    = -1;

    Feature& f = *feature.data();

    // Reduce operation will collect the local minimums for each thread and then combine them in the global minimums
    #pragma omp parallel firstprivate( localMinimumDistance, localMinimumIndex )
    {
        #pragma omp for
        for( int i = 0; i < _grid->capacity(); i++ )
        {
            // Calculate the distance between the input feature and the map feature at index i
            double distance = f.distance( _grid->item(i) );

            if( distance  < localMinimumDistance )
            {
                localMinimumDistance = distance;
                localMinimumIndex = i;
            }
        }

        #pragma omp critical
        {
            /* If the thread's local minimum feature is closer than the global minimum
             * make the local closest feature the global closest feature
             */
            if( localMinimumDistance < globalMinimumDistance )
            {
                globalMinimumDistance = localMinimumDistance;
                globalMinimumIndex    = localMinimumIndex;
            }
        }
    }

    return globalMinimumIndex;
}



void SOM::update( FeaturePtr feature )
{
    _normalizer->normalizeFeature( feature );

    // Find the neighborhood in the SOM that of the feature that is the closest to the input feature
    QVector< QPair<int, int> > neighbors = _grid->neighborhood( weights.size() - 1, closestFeature( feature ));

    // Iterate over all features in the neighborhood and update them to make them more similar to the training feature.
    #pragma omp parallel for
    for( int i=0; i<neighbors.size(); i++ )
    {
        int index     = neighbors[i].first;
        int distance  = neighbors[i].second;

        FeaturePtr gridFeature = _grid->item( index );
        gridFeature->adjust( feature, weights[ distance ] );
    }
}



FeatureGrid SOM::grid()
{
    return _grid;
}



NormalizerPtr SOM::normalizer()
{
    return _normalizer;
}

} // namespace
