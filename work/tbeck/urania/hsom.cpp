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

QVector<FeaturePtr> HSOM::extractFeatures( QVector<SuspectPtr>& suspects )
{
    QVector<FeaturePtr> features;
    foreach( SuspectPtr suspect, suspects )
        foreach( FeaturePtr feature, suspect->features() )
            features.append( feature );
    return features;
}

void HSOM::calculateFeatureNormalization( QVector<FeaturePtr>& features, double epsilon, int sigmaStep )
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

void HSOM::normalizeFeatures( QVector<FeaturePtr>& features )
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

void HSOM::trainANN( const QVector<SuspectPtr>& suspects, int annIters, double annEps )
{
    int inputSize = _som->size();

    // Fetch the number of categories into which the suspects may be classified.  This is the output size of the ANN
    int outputSize = Suspect::categoryCount();
    SOMError::requireCondition( outputSize > 0, "Invalid output size" );

    /* Make an input matrix for training the back-end MLPANN.
     * This is a matrix composed of vectors the length of the SOM's area ( width x height )
     */
    cv::Mat_<double>  input( suspects.size(), inputSize, 0.0 );

    /* Make an output matrix for trainig the back-end MLPANN.
     * This is a matrix composed of vectors the length of the number of classification categories.
     */
    cv::Mat_<double> output( suspects.size(), categoryCount, 0.0 );

    cv::Mat_<double> inRow, outRow;

    for( int i=0; i<suspects.size(); i++ )
    {
        SuspectPtr suspect = trainingSuspects[i];
        HistogramPtr histogram = suspect->histogram();

        for( int j=0; j<Suspect::categoryCount(); j++ )
            inRow[i] = histogram[i];
    }

    cv::TermCriteria terminationCriteria;
    terminationCriteria.type = 0;
    if( annIters != 0 )
        terminationCriteria.type += cv::TermCriteria::MAX_ITER;
    if( annEps != 0.0 )
        terminationCriteria.type += cv::TermCriteria::EPS;

    cv::ANN_MLP_TrainParams trainingParams( terminationCriteria, CvANN_MLP_TrainParams::RPROP, annIters, annEps );

    int normFlag = 0;

    int tmp[4];
    tmp[0] = inputSize;
    tmp[1] = tmp[0]/2;
    tmp[2] = tmp[1]/2;
    tmp[3] = outputSize;

    CvMat ls = cvMat( 4, 1, CV_32S, tmp );

//    cv::Mat_<int> layerSizes( 4, 1, CV_8U );                                                                          /// @todo  Make this more dynamic, so that the size and count of input layers is dependent on the number of ANN inputs
//    layerSizes(0,0) = grid.l();                                                                                       // Input layer.
//    layerSizes(1,0) = grid.l()/2;                                                                                     // First hidden layer is 1/2 the size of the input layer
//    layerSizes(2,0) = grid.l()/4;                                                                                     // Second hidden layer is 1/4 the size of the input layer
//    layerSizes(3,0) = _catCt;                                                                                         // The output layer has one node for each category

    if( _ann != NULL )
        delete _ann;
    _ann = new CvANN_MLP( &ls, CvANN_MLP::SIGMOID_SYM );

    _ann->train( input, output, cv::Mat(), cv::Mat(), trainingParams, normFlag );
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
        trainANN( suspects, annIterations, annEps );
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
    cv::Mat_<double>  input( 1, _som->size(), 0.0 );
    cv::Mat_<double> output( 1, Suspect::categoryCount(), 0.0 );

    QVector<FeaturePtr> features = suspect->features();
    normalizeFeatures( features );

    generateHistogram( suspect );
    HistogramPtr histogram = suspect->histogram();

    for( int i=0; i<histogram->size(); i++ )
        input[i] = histogram[i];

    _ann->predict( input, output );

    QVector<double> classification;
    for( int i=0; i<Suspect::categoryCount(); i++ )
        classification.push_back( output[i] );

    suspect->setClassification( classification );
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
