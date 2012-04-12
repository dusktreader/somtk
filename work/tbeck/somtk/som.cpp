#include "som.h"

namespace hsom {

SOM::SOM( Grid<Feature>& grid ) : grid( grid )
{}

SOM::~SOM(){}

void SOM::initializeTraining( QMap<QString, QVariant> somParameters, NormalizerPtr normalizer, int featureSize )
{
    bool ok = true;

    // Describes the number of epochs of training the SOM will need
    ASSERT_MSG( somParameters.contains( "maxEpochs" ), "SOM parameters doesn't contain epoch count" );
    maxEpochs = somParameters["maxEpochs"].toInt( &ok );
    ASSERT_MSG( ok, "Couldn't convert maximum epoch count to an int" );

    // Describes the inital training weight for the SOM
    ASSERT_MSG( somParameters.contains( "initialAlpha" ), "SOM parameters doesn't contain initial alpha" );
    initialAlpha = somParameters["initialAlpha"].toDouble( &ok );
    /// @todo  Add range check to alpha (hint on values)
    /// @todo  Perhaps use default values if the parameters aren't specified
    ASSERT_MSG( ok, "Couldn't convert initial alpha to a double" );

    // Describes the inital radius of the training neighborhood for the SOM
    ASSERT_MSG( somParameters.contains( "initialRadiusRatio" ), "SOM parameters doesn't contain initial radius ratio" );
    double initialRadiusRatio = somParameters["initialRadiusRatio"].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convvert initial radius ratio to a double" );
    ASSERT_MSG( initialRadiusRatio < 0.5, "Initial radius ratio may not exceed 0.5" );
    ASSERT_MSG( initialRadiusRatio > 0.0, "Initial radius ratio must be greater than 0" );


    /// @todo  determine if there should be other constraints to alpha and radius ratio (negatives, ffs!)

    alpha_tf = 0.10;   // This is a tuning factor...should not be hardcoded
    alpha_Nf = 0.25;   // This is a tuning factor...should not be hardcoded
    alpha_gamma = -log( alpha_Nf ) / ( alpha_tf * maxEpochs );

    initialRadius = initialRadiusRatio * grid.diagonal();
    radius_tf = 0.25;   // This is a tuning factor...should not be hardcoded
    radius_Nf = 0.50;   // This is a tuning factor...should not be hardcoded
    radius_gamma = -log( radius_Nf ) / ( radius_tf * maxEpochs );

    currentEpoch = -1;
    nextEpoch();

    for( int i=0; i<grid.capacity(); i++ )
    {
        Feature newFeature( featureSize );
        normalizer->setFeature( newFeature );
        grid[i] = newFeature;
    }
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



int SOM::closestFeature( Feature feature )
{
    double globalMinimumDistance = DBL_MAX;
    int    globalMinimumIndex    = -1;

    double localMinimumDistance = DBL_MAX;
    int    localMinimumIndex    = -1;

    // Reduce operation will collect the local minimums for each thread and then combine them in the global minimums
    #pragma omp parallel firstprivate( localMinimumDistance, localMinimumIndex )
    {
        #pragma omp for
        for( int i = 0; i < grid.capacity(); i++ )
        {
            // Calculate the distance between the input feature and the map feature at index i
            double distance = feature.distance( grid[i] );

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


void SOM::update( Feature feature )
{
    // Find the neighborhood in the SOM that of the feature that is the closest to the input feature
    QVector< QPair<int, int> > neighbors = grid.neighborhood( weights.size(), closestFeature( feature ));

    // Iterate over all features in the neighborhood and update them to make them more similar to the training feature.
    #pragma omp parallel for
    for( int i=0; i<neighbors.size(); i++ )
    {
        int index     = neighbors[i].first;
        int distance  = neighbors[i].second;
        grid[index].adjust( feature, weights[ distance ] );
    }
}



void SOM::train( QVector<Feature> features, NormalizerPtr normalizer, QMap<QString, QVariant> somParameters, bool skipInit_debugOnly )
{
    if( skipInit_debugOnly == false )
        initializeTraining( somParameters, normalizer, features.front().size() );

    do
    {
       // Shuffle the list of features to remove bias
        std::random_shuffle( features.begin(), features.end() );

        ;

        // Update the SOM with each feature from the globalFeatures list.
        // @todo: consider random sampling for the training if the number of features is high
        foreach( Feature feature, features )
            update( feature );

    } while( nextEpoch() != false );

}

QVector<Feature> SOM::dumpFeatures()
{
    return grid.items();
}

} // namespace hsom
