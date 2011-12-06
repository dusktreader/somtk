#include "hsom.h"

namespace hsom {

HSOM::HSOM( QObject* parent ) :
    QThread( parent )
{
    ann = NULL;
}

HSOM::HSOM( SOMPtr som, CvANN_MLP* ann, QObject* parent ) :
    QThread( parent ),
    _som( som ),
    _ann( ann )
{}

HSOM::~HSOM()
{}

void HSOM::clearSuspects()
{
    suspects.clear();
    normMean.clear();
    normStdv.clear();
    normAlpha.clear();
}

void HSOM::normalizeFeatures( QList<FeaturePtr> &features, double epsilon, int sigmaStep, unsigned maxFeatures )
{
    unsigned featureSize = features.first()->size();

    normMean  = QVector<double>( featureSize, 0.0 );
    normStdv  = QVector<double>( featureSize, 0.0 );
    normAlpha = QVector<double>( featureSize, 0.0 );

    /// @todo  these should probably be parameterized....
    // double eps = 0.0625;
    // int sigmaStep = 2;
    unsigned featureCount = 0;

    foreach( FeaturePtr feature, features )
    {
        if( featureCount >= maxFeatures )
            break;

        for( int i = 0; i < featureSize; i++ )
        {
            double t = feature->at( i ) - normMean[i];
            normMean[i] += t / featCt;
            normStdv[i] += t * ( feature->at( i ) - normMean[i] );
        }
        featureCount++;
    }

    for( int i = 0; i < featureSize; i++ )
    {
        normStdv[i] = sqrt( normStdv[i] / ( featureCount - 1 ) );
        normAlpha[i] = log( 1 / epsilon - 1 ) / ( sigmaStep * normStdv[i] );
    }


    foreach( FeaturePtr feature, features )
    {
        for( int i = 0; i < featureSize; i++ )
        {
            double* t = *(feature->data())[i];
            t = 1 / ( 1 + exp( -normAlpha[i] * ( t - normMean[i] ) ) );
        }
    }
}

void HSOM::trainSOM( int somEpochs, double initAlpha, double initRadRat )
{
    ASSERT_MSG( initRadRat <= 0.5, "Inital radius ratio may not exceed 1/2" );

    double radius;
    double radius0 = initRadRat * grid.size().w;                                                                        /// @todo  Make radius_tf, radius_Nf, alpha_tf, alpha_Nf arguments to this function
    double radius_tf = 0.25;  // This is a tuning factor...should not be hardcoded
    double radius_Nf = 0.5;   // This is a tuning factor...should not be hardcoded
    double radius_gamma = -log( radius_Nf ) / ( radius_tf * somEpochs );

    double alpha;
    double alpha0 = initAlpha;
    double alpha_tf = 0.1;  // This is a tuning factor...should not be hardcoded
    double alpha_Nf = 0.25; // This is a tuning factor...should not be hardcoded
    double alpha_gamma = -log( alpha_Nf ) / ( alpha_tf * somEpochs );

    QList<FeaturePtr> globalFeatures;
    foreach( SuspectPtr suspect, trainingSuspects )
        globalFeatures.append( suspect->features() );



    for( int epoch=0; epoch<somEpochs; epoch++ )
    {
        random_shuffle( suspects.begin(), suspects.end() );                                                             // Shuffle the list of suspects so that representatives from each category appear uniformly throught the training

        alpha = alpha0 * exp( -alpha_gamma * epoch );
        radius = radius0 * exp( -radius_gamma * epoch );
        preCalcWeights( alpha, radius );                                                                                // Pre-calculate the gaussian weights for updating the SOM
        for( unsigned int i=0; i<suspects.size(); i++ )
        {
            ASSERT( statusCheck( i, "Processing " + suspects[i]->name() ) );
            updateSOM( suspects[i] );
        }
    }
}

void HSOM::generateHistograms()
{
    ASSERT( statusCheck( 0, "---", "Generating histograms" ) );

    for( unsigned int i=0; i<suspects.size(); i++ )
    {
        ASSERT( statusCheck( i, "Processing " + suspects[i]->name() ) );
        updateHistogram( suspects[i] );
    }
}

void HSOM::trainANN( int annIters, double annEps )
{
    ASSERT( statusCheck( 0, "---", "Training ANN" ) );

    cv::Mat_<double>  input( suspects.size(), grid.l(), 0.0 );                                                          // Make an input matrix for training the back-end MLPANN.  This is a matrix composed of vectors of size l = grid width x grid height
    cv::Mat_<double> output( suspects.size(), _catCt, 0.0 );                                                            // Make an output matrix for trainig the back-end MLPANN.  This is a matrix composed of vectors of size catCt.

    cv::Mat_<double> inRow, outRow;

    for( unsigned int i=0; i<suspects.size(); i++ )
    {
        ASSERT( statusCheck( i, "Extracting ANN inputs from " + suspects[i]->name() ) );
        inRow = input.row( i );
        outRow = output.row( i );
        suspects[i]->setANNVectors( inRow, outRow );
    }

    cv::TermCriteria tc;
    tc.type = 0;
    if( annIters != 0 )
        tc.type += cv::TermCriteria::MAX_ITER;
    if( annEps != 0.0 )
        tc.type += cv::TermCriteria::EPS;

    cv::ANN_MLP_TrainParams tParams( tc, CvANN_MLP_TrainParams::RPROP, annIters, annEps );

    int normFlag = 0;

    int tmp[4];
    tmp[0] = grid.l();
    tmp[1] = grid.l()/2;
    tmp[2] = grid.l()/4;
    tmp[3] = _catCt;
    CvMat ls = cvMat( 4, 1, CV_32S, tmp );

//    cv::Mat_<int> layerSizes( 4, 1, CV_8U );                                                                          /// @todo  Make this more dynamic, so that the size and count of input layers is dependent on the number of ANN inputs
//    layerSizes(0,0) = grid.l();                                                                                       // Input layer.
//    layerSizes(1,0) = grid.l()/2;                                                                                     // First hidden layer is 1/2 the size of the input layer
//    layerSizes(2,0) = grid.l()/4;                                                                                     // Second hidden layer is 1/4 the size of the input layer
//    layerSizes(3,0) = _catCt;                                                                                         // The output layer has one node for each category

    if( ann != NULL )
        delete ann;
    ann = new CvANN_MLP( &ls, CvANN_MLP::SIGMOID_SYM );

    ann->train( input, output, cv::Mat(), cv::Mat(), tParams, normFlag );
}

void HSOM::train( int somEpochs, double initAlpha, double initR, int annIters, double annEps )
{
    random_shuffle( suspects.begin(), suspects.end() );
    try
    {
        analyzeSuspects();
        trainSOM( somEpochs, initAlpha, initR );
        generateHistograms();
        trainANN( annIters, annEps );
        statusCheck( -1 );
    }
    catch( ... )
    {
        statusCheck( -1 );
        throw;
    }
}

double HSOM::report( string &text )
{
    text.clear();
    double hitRate = 0.0;
    string hitReport, missReport;
    int suspectCt = suspects.size();
    cv::Mat_<int> confusionMatrix( _catCt, _catCt, 0 );

    for( unsigned int i=0; i<suspects.size(); i++ )
    {

        confusionMatrix( suspects[i]->predCat(), suspects[i]->realCat() )++;
        if( suspects[i]->realCat() == suspects[i]->predCat() )
        {
            hitReport.append( suspects[i]->name() + " --> Class " + num2str( suspects[i]->predCat() ) + "\n" );
            hitRate++;
        }
        else
        {
            missReport.append( suspects[i]->name() + " --> Class " + num2str( suspects[i]->realCat() ) + "\n" );
            missReport.append( "  votes: <" );
            vector<double> cats = suspects[i]->cats();
            for( int i=0; i<_catCt; i++ )
                missReport.append( num2str(cats[i]) + "  " );
            missReport.append( "\n" );
        }
    }

    text.append( "Hits\n----\n" + hitReport );
    text.append( "\nMisses\n------\n" + missReport );

    text.append( "\n\nConfusion Matrix\n----------------\n\n" );
    text.append( "              Actual\n" );
    for( int i=-2; i<_catCt; i++ )
    {
        if( i < 0 )
            text.append( "             " );
        else
        {
            if( i == _catCt / 2 )
                text.append( "Predicted" );
            else
                text.append( "         " );
            text.append( num2str( i, 0, 3 ) + "|" );
        }
        for( int j=0; j<_catCt; j++ )
        {
            switch( i )
            {
                case -2:
                    text.append( num2str( j, 0, 5 ) );
                    break;
                case -1:
                    text.append( "-----" );
                    break;
                default:
                    text.append( num2str( confusionMatrix(i,j), 0, 5 ) );
            }
        }
        text.append( "\n" );
    }

    hitRate /= suspectCt;
    return hitRate;
}

void HSOM::classify()
{
    string title = "Analyzing with " + _name;
    ASSERT( statusCheck( 0, title + "__size_buffer__", title, suspects.size() ) );

    cv::Mat_<double>  input( 1, grid.l(), 0.0 );
    cv::Mat_<double> output( 1, _catCt, 0.0 );

    for( unsigned int i=0; i<suspects.size(); i++ )
    {
        ASSERT( statusCheck( i, "Classifying " + suspects[i]->name() ) );
        classify( suspects[i], input, output );
    }
}

void HSOM::classify( Suspect* suspect, cv::Mat_<double>& input, cv::Mat_<double>& output )
{
    if( input.empty() )
    {                                                                                                                   // If the input and output matrix arent' specified, they need to be allocated and de-allocated in this function
        input  = cv::Mat_<double>( 1, grid.l(), 0.0 );
        output = cv::Mat_<double>( 1, _catCt, 0.0 );
    }

    updateHistogram( suspect );                                                                                         // Update the histogram with features from the suspect
    suspect->setANNVectors( input, output );                                                                             // Set the inputs for the back-end ANN classifier
    ann->predict( input, output );                                                                                      // Classify the suspect using the histogram and the ANN classifier
    suspect->setCats( output );                                                                                          // Update the vote vector for the suspect based on the output of the ANN
}

int HSOM::closestFeatureIndex( Feature* feat )
{
    double globalMinDist = DBL_MAX;
    int    globalMinIdx = -1;

    double localMinDist = DBL_MAX;
    int    localMinIdx = -1;
    #pragma omp parallel firstprivate( localMinDist, localMinIdx )
    {
        #pragma omp for
        for( int i=0; i<grid.l(); i++ )
        {
            double dist = feat->distance( grid[i] );                                                                        // Calculate the distance between the input feature and the map feature at index i
            if( dist  < localMinDist )
            {
                localMinDist = dist;
                localMinIdx = i;
            }
        }
        #pragma omp critical
        {
            if( localMinDist < globalMinDist )                                                                          // If the thread's local minimum feature is closer than the global minimum, make the local closest feature the global closest feature
            {
                globalMinDist = localMinDist;
                globalMinIdx  = localMinIdx;
            }
        }
    }
    return globalMinIdx;
}

void HSOM::updateSOM( Suspect* suspect )
{
    vector<int> neighbors;
    Feature* feat = suspect->getNextFeature();
    while( feat != NULL )                                                                                               // While the suspect has features to provide
    {
        PointPlus<int> pt = closestFeatureCoords( feat );                                                               // Find the feature that is closest to the training feature
        neighbors = grid.neighborhood( weights.size(), pt );                                                            // Get all indices of features within the neighborhood radius.
        #pragma omp parallel for
        for( unsigned int i=0; i<neighbors.size(); i++ )
        {                                                                                                               // Iterate over all features in the neighborhood and update them to make them more similar to the training feature.
            int nIdx = neighbors[i];
            grid[ nIdx ]->adjust( feat, weights[ grid.distance( nIdx ) ] );
        }
        feat = suspect->getNextFeature();                                                                               // Get the next feature in the suspect
    }
}

void HSOM::updateHistogram( Suspect* suspect )
{
    int idx;
    Feature* feat = suspect->getNextFeature();                                                                          // Fetch the first feature from the suspect
    while( feat != NULL )                                                                                               // While the suspect still has features to count,
    {
        idx = closestFeatureIndex( feat );                                                                              // Find the feature that is closest to the training feature
        suspect->incrementHistogram( idx );                                                                             // Increment the corresponding historam bin
        feat = suspect->getNextFeature();                                                                               // Get the next feature in the suspect
    }
}

string HSOM::name()
{
    return _name;
}

int HSOM::catCt()
{
    return _catCt;
}

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

} // namespace hsom
