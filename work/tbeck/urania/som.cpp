#include "som.h"

SOM::SOM(){}



SOM::SOM( const SizePlus<int>& size )
{
    grid = HexGrid<FeaturePtr>( size );
}



SOM::~SOM()
{
    grid.clear();
}



void SOM::initializeTraining( int epochCount, double initalAlpha, double initialRadiusRatio, FeaturePtr featureRep )
{
    /// @todo  Make radius and alpha tuning values parameters

    ASSERT_MSG( initialRadiusRatio < 0.5, "Initial radius ratio may not exceed 0.5" );

    /// @todo  determine if there should be other constraints to alpha and radius ratio (negatives, ffs!)

    this->maxEpochs = epochCount;

    this->initialAlpha = initialAlpha;
    alpha_tf = 0.1;
    alpha_Nf = 0.25;
    alpha_gamma = -log( alpha_Nf ) / ( alpha_tf * epochCount );

    initialRadius = initialRadiusRatio * grid.size().w;
    radius_tf = 0.1;
    radius_Nf = 0.25;
    radius_gamma = -log( radius_Nf ) / ( radius_tf * epochCount );

    currentEpoch = -1;
    nextEpoch();

    this->featureRep = featureRep;

    PointPlus<int> pt;
    for( pt.y = 0; pt.y < grid.size().h; pt.y++ )
        for( pt.x = 0; pt.x < grid.size().w; pt.x++ )
            grid[pt] = featureRep->generateFeature( pt );
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

    double twoSigmaSquared = 2 * pow( sigma, 2 );

    weights = vector<double>( (int)radius + 1 );

    /* Calculate the gaussian weighting function.
     * The function is dependent on alpha, sigma, and the current distance from the center of the neighborhood
     */
    for( unsigned i = 0; i < weights.size(); i++ )
        weights[i] = alpha * exp( -pow( i, 2.0 ) / twoSigmaSquared );
}



void SOM::closestFeatureIndex( FeaturePtr feature )
{
    double globalMinimumDistance = DBL_MAX;
    int    globalMinimumIndex    = -1;

    double localMinimumDistance = DBL_MAX;
    int    localMinimumIndex    = -1;

    // Reduce operation will collect the local minimums for each thread and then combine them in the global minimums
    #pragma omp parallel firstprivate( localMinimumDistance, localMinimumIndex )
    {
        #pragma omp for
        for( int i = 0; i < grid.l(); i++ )
        {
            // Calculate the distance between the input feature and the map feature at index i
            double distance = feature->distance( grid[i] );

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



PointPlus<int> SOM::closestFeatureCoords( FeaturePtr feature )
{
    return grid.coords( closestFeatureIndex( feature ) );
}



void SOM::train( FeaturePtr feature )
{
    // Find the feature in the SOM that is the closest to the input feature
    PointPlus<int> c = closestFeatureCoords( feature );

    vector<int> neighbors = grid.neighborhood( weights.size(), c );

    // Iterate over all features in the neighborhood and update them to make them more similar to the training feature.
    #pragma omp parallel for
    for( unsigned i = 0; i < neighbors.size(); i++ )
    {
        int index = neighbors[i];
        grid[index]->adjust( feature, weights[ grid.distance( index ) ] );
    }
}
