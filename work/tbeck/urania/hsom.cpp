#include "hsom.h"

namespace hsom {

HSOM::HSOM( QObject* parent ) :
    QThread( parent ),
    _ann( NULL )
{}



HSOM::HSOM( SOMPtr som, QObject* parent ) :
    QThread( parent ),
    _som( som ),
    _ann( NULL )
{}



HSOM::~HSOM()
{
    clear();
}



void HSOM::clear()
{
    if( _ann != NULL )
    {
        delete _ann;
        _ann = NULL;
    }
    normMean.clear();
    normStdv.clear();
    normAlpha.clear();
}



QVector<FeaturePtr> HSOM::extractFeatures( QVector<SuspectPtr> suspects )
{
    QVector<FeaturePtr> features;
    foreach( SuspectPtr suspect, suspects )
        foreach( FeaturePtr feature, suspect->features() )
            features.append( feature );
    return features;
}



void HSOM::calculateFeatureNormalization( QVector<FeaturePtr> features, double epsilon, int sigmaStep )
{
    unsigned featureSize = features.first()->size();

    normMean  = QVector<double>( featureSize, 0.0 );
    normStdv  = QVector<double>( featureSize, 0.0 );
    normAlpha = QVector<double>( featureSize, 0.0 );

    foreach( FeaturePtr feature, features )
    {
        for( int i = 0; i < featureSize; i++ )
        {
            double t = feature->data()[i] - normMean[i];
            normMean[i] += t / featCt;
            normStdv[i] += t * ( feature->data()[i] - normMean[i] );
        }
    }

    for( int i = 0; i < featureSize; i++ )
    {
        normStdv[i] = sqrt( normStdv[i] / ( features.size() - 1 ) );
        normAlpha[i] = log( 1 / epsilon - 1 ) / ( sigmaStep * normStdv[i] );
    }
}



void HSOM::normalizeFeatures( QVector<FeaturePtr> features )
{
    SOMError::requireCondition( normMean.empty() == false, "Feature normalization statistics not been computed yet" );

    foreach( FeaturePtr feature, features )
        normalizeFeature( feature );
}



void HSOM::normalizeFeature( FeaturePtr feature )
{
    for( int i = 0; i < feature->size(); i++ )
    {
        double& t = feature->data()[i];
        t = 1 / ( 1 + exp( -normAlpha[i] * ( t - normMean[i] ) ) );
    }

}



void HSOM::trainSOM( QVector<FeaturePtr>& features, int epochCount, double initialAlpha, double initialRadiusRatio )
{
    SOMError::requireCondition( initialRadiusRatio <= 0.5, "Inital radius ratio may not exceed 1/2" );

    _som->initializeTraining( epochCount, initialAlpha, initialRadiusRatio, globalFeatures.first() );

    do
    {
        // Shuffle the list of features to remove bias
        random_shuffle( features.begin(), features.end() );

        // Update the SOM with each feature from the globalFeatures list.
        // @todo: consider random sampling for the training if the number of features is high
        foreach( FeaturePtr feature, features )
            _som->train( feature );

    } while( _som->nextEpoch() != false );
}



void HSOM::generateHistograms( QVector<SuspectPtr>& suspects )
{
    for( int i=0; i<suspects.size(); i++ )
        generateHistogram( trainingSuspects[i] );
}



void HSOM::generateHistogram( SuspectPtr suspect )
{
    foreach( FeaturePtr feature, suspect->features() )
    {
        // Find the feature that is closest to the input feature
        QPoint pt = _som->closestFeatureCoords( feature );

        // Increment the suspect's histogram bin at the point of the closest feature
        suspect->histogram()->increment( pt );
    }
}



void HSOM::train( QVector<SuspectPtr>& suspects,
                  int epochCount, double initialAlpha, double initialRadiusRatio, int annIterations, double annEps )
{
    try
    {
        QVector<FeaturePtr> features = extractFeatures( suspects );
        calculateFeatureNormalization( features ); // epsilon, sigmaStep, maxFeatures
        normalizeFeatures( features );
        trainSOM( features, epochCount, initialAlpha, initialRadiusRatio );
        generateHistograms( suspects );
        classifier->train( suspects );
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

    QVector<FeaturePtr> features = suspect->features();
    normalizeFeatures( features );

    generateHistogram( suspect );

    classifier->classify( suspect );
}


/*
void HSOM::save( const string& fileName )
{
    cv::FileStorage fs( fileName, cv::FileStorage::WRITE );
    _name = fileName;
    DB_REP_VAR( fileName );
    write( fs );
}

void HSOM::load( const string& fileName )
{
    cv::FileStorage fs( fileName, cv::FileStorage::READ );
    _name = fileName;
    cv::FileNode root = fs.root();
    read( root );
}

void HSOM::read( cv::FileNode& fn )
{
    clear();
    cv::FileNode root = fn[alias];
    _catCt = (int)root["catCt"];
    grid.setSize( SizePlus<int>( (int)root["w"], (int)root["h"] ) );
    initFeatures();

    for( int i=0; i<grid.l(); i++ )
    {
        grid[i]->read( root["feats"][i] );
    }

    if( ann == NULL )
        ann = new CvANN_MLP();
    ann->read( const_cast<CvFileStorage*>(fn.fs), const_cast<CvFileNode*>(root["ann"].node) );
}

void HSOM::write( cv::FileStorage& fs )
{
    DB_REP;
    fs << alias << "{";
        fs << "catCt" << _catCt;
        fs << "w"     << grid.size().w;
        fs << "h"     << grid.size().h;
        fs << "feats" << "{";
        for( int i=0; i<grid.l(); i++ )
            grid[i]->write( fs );
        ann->write( fs.fs, "ann" );
    fs << "}";
}
*/

} // namespace hsom
