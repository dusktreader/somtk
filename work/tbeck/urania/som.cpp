#include "som.h"

namespace hsom {

SOM::SOM(){}



SOM::SOM( const QSize& size )
{
    grid = HexGrid<FeaturePtr>( size );
}



SOM::~SOM()
{
    grid.clear();
}



const QSize& SOM::size()
{
    return grid.size();
}



void SOM::initializeTraining( QMap<QString, QVariant> somParameters, FeaturePtr featureRep )
{
    bool ok = true;

    // Describes the number of epochs of training the SOM will need
    ASSERT_MSG( somParameters.contains( "maxEpochs" ), "SOM parameters doesn't contain epoch count" );
    maxEpochs = somParameters["maxEpochs"].toInt( &ok );
    ASSERT_MSG( ok, "Couldn't convert maximum epoch count to an int" );

    // Describes the inital training weight for the SOM
    ASSERT_MSG( somParameters.contains( "initialAlpha" ), "SOM parameters doesn't contain initial alpha" );
    initialAlpha = somParameters["initialAlpha"].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convvert initial alpha to a double" );

    // Describes the inital radius of the training neighborhood for the SOM
    ASSERT_MSG( somParameters.contains( "initialRadiusRatio" ), "SOM parameters doesn't contain initial radius ratio" );
    double initialRadiusRatio = somParameters["initialRadiusRatio"].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convvert initial radius ratio to a double" );
    ASSERT_MSG( initialRadiusRatio < 0.5, "Initial radius ratio may not exceed 0.5" );
    ASSERT_MSG( initialRadiusRatio > 0.0, "Initial radius ratio must be greater than 0" );


    /// @todo  determine if there should be other constraints to alpha and radius ratio (negatives, ffs!)

    alpha_tf = 0.10;   // This is a tuning factor...should not be hardcoded
    alpha_Nf = 0.25;   // This is a tuning factor...should not be hardcoded
    alpha_gamma = -log( alpha_Nf ) / ( alpha_tf * epochCount );

    initialRadius = initialRadiusRatio * grid.size().w;
    radius_tf = 0.25;   // This is a tuning factor...should not be hardcoded
    radius_Nf = 0.50;   // This is a tuning factor...should not be hardcoded
    radius_gamma = -log( radius_Nf ) / ( radius_tf * epochCount );

    currentEpoch = -1;
    nextEpoch();

    this->featureRep = featureRep;

    QPoint pt;
    for( pt.setY( 0 ); pt.y() < grid.size().height(); pt.setY( pt.y() + 1 ) )
        for( pt.setX( 0 ); pt.x() < grid.size().width(); pt.setX( pt.x() + 1 ) )
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



QPoint SOM::closestFeatureCoords( FeaturePtr feature )
{
    return grid.coords( closestFeatureIndex( feature ) );
}



void SOM::update( FeaturePtr feature )
{
    // Find the feature in the SOM that is the closest to the input feature
    QPoint closest = closestFeatureCoords( feature );

    QVector<int> neighbors = grid.neighborhood( weights.size(), closest );

    // Iterate over all features in the neighborhood and update them to make them more similar to the training feature.
    #pragma omp parallel for
    for( int i=0; i<neighbors.size(); i++ )
    {
        int index = neighbors[i];
        grid[index]->adjust( feature, weights[ grid.distance( index ) ] );
    }
}



void SOM::train( QVector<FeaturePtr> features, QMap<QString, QVariant> somParameters )
{
    initializeTraining( somParameters, features.first() );

    do
    {
        // Shuffle the list of features to remove bias
        random_shuffle( features.begin(), features.end() );

        // Update the SOM with each feature from the globalFeatures list.
        // @todo: consider random sampling for the training if the number of features is high
        foreach( FeaturePtr feature, features )
            update( feature );

    } while( nextEpoch() != false );

}

} // namespace hsom
