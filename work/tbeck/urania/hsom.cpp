#include "hsom.h"

using namespace std;

string HSOM::alias = "HSOM";

HSOM::HSOM() : HexGrid()
{
    catCt = -1;
    ann = NULL;
}

HSOM::HSOM( const SizePlus<int>& sz, int catCt )
{
    grid = HexGrid( sz );
    this->catCt = catCt;
    ann = NULL;
    name = "unnamed";
}

HSOM::~HSOM()
{
    clear();
    delete ann;
}

void HSOM::clear()
{
    clearSuspects();
    grid.clear();
}

void HSOM::clearSuspects()
{
    for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ )
        delete *s;
    suspects.clear();
}

bool HSOM::statusCheck( int iteration, string msg1, string msg2, int maxIters )
{
    cout << "HSOM training " << iteration << "/" << maxIters << ": " << msg1 << " ... " << msg2 << endl;
    return true;
}

bool HSOM::analyzeSuspects()
{
    return true;
}

void HSOM::trainSOM( int somEpochs, double initAlpha, double initRadRat )
{
    ASSERT_MSG( initRadRat <= 0.5, "Inital radius ratio may not exceed 1/2" );

    bool alive = statusCheck( 0, "Beginning SOM training...", "Training SOM" );
    if( !alive )
        return false;

    double radius;
    double radius0 = initRadRat * w;                                                                                    /// @todo  Make radius_tf, radius_Nf, alpha_tf, alpha_Nf arguments to this function
    double radius_tf = 0.25;
    double radius_Nf = 0.5;
    double radius_gamma = -log( radius_Nf ) / ( radius_tf * somEpochs );

    double alpha;
    double alpha0 = initAlpha;
    double alpha_tf = 0.1;
    double alpha_Nf = 0.25;
    double alpha_gamma = -log( alpha_Nf ) / ( alpha_tf * somEpochs );

    for( int E=0; E<somEpochs; E++ )
    {
        ASSERT( statusCheck( 0, "---", "SOM Epoch " + num2str( E + 1 ) + "/" + num2str( somEpochs ) ) );

        random_shuffle( suspects.begin(), suspects.end() );                                                             // Shuffle the list of suspects so that representatives from each category appear uniformly throught the training

        alpha = alpha0 * exp( -alpha_gamma * E );
        radius = radius0 * exp( -radius_gamma * E );
        preCalcWeights( alpha, radius );                                                                                // Pre-calculate the gaussian weights for updating the SOM
        int i = 0;
        for( unsigned int i=0; i<suspects.size(); i++ )
        {
            ASSERT( statusCheck( i, "Processing " + suspects[i]->getName() ) );
            updateSOM( suspects[i] );
        }

        /** @todo verify that the previous loop is functional
        /*for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ ){                               // Iterate over all training suspects
            alive = statusCheck( i++, "Processing " + (*s)->getName() );
            if( !alive ){
                RETURN false;
            }
            updateSOM( *s );
        }*/
    }
}

void HSOM::generateHistograms()
{
    ASSERT( statusCheck( 0, "---", "Generating histograms" ) );

    int i = 0;
    for( unsigned int i=0; i<suspects.size(); i++ )
    {
        alive = statusCheck( i++, "Processing " + suspects[i]->getName() );
        if( !alive )
            return false;
        updateHistogram( suspects[i] );
    }

    /*for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ ){                                     // Iterate over all training suspects
        alive = statusCheck( i++, "Processing " + (*s)->getName() );
        if( !alive ){
            RETURN false;
        }
        updateHistogram( *s );                                                                                          // Update each suspects's histogram now that the SOM part is fully trained.
    }*/
}

bool HSOM::trainANN( int annIters, double annEps )
{
    ASSERT( statusCheck( 0, "---", "Training ANN" ) );

    int l = grid.size().area();

    cv::Mat_<double>  input( suspects.size(), l, 0.0 );                                                                 // Make an input matrix for training the back-end MLPANN.  This is a matrix composed of vectors of size l = grid width x grid height
    cv::Mat_<double> output( suspects.size(), catCt, 0.0 );                                                             // Make an output matrix for trainig the back-end MLPANN.  This is a matrix composed of vectors of size catCt.

    cv::Mat_<double> inRow, outRow;

    for( unsigned int i=0; i<suspects.size(); i++ )
    {
        ASSERT( statusCheck( i++, "Extracting ANN inputs from " + suspects[i]->getName() ) );
        inRow = input.row( i );
        outRow = output.row( i );
        suspects[i]->setANNVectors( inRow, outRow );
    }

    /// @todo verify that new implementation works correctly
/*
    double* inPtr  = (double*)input.data;                                                                               // These two utility pointers will point to the current locations within the input and output matrices
    double* outPtr = (double*)output.data;
    for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ ){                                     // Iterate over all training suspects
        (*s)->setANNVectors( inPtr, l, outPtr, catCt );                                                                 // Update the input and output matrices with values from the suspect's histogram
        inPtr += l;
        outPtr += catCt;
    }*/

    int tcType = 0;
    if( annIters != 0 )
        tcType += cv::TermCriteria::MAX_ITER;
    if( annEps != 0.0 )
        tcType += cv::TermCriteria::EPS;

    cv::ANN_MLP_TrainParams tParams( tcType, CvANN_MLP_TrainParams::RPROP, annIters, annEps );

    int normFlag = 0;

    cv::Mat_<int> layerSizes( 4, 1 );                                                                                   /// @todo  Make this more dynamic, so that the size and count of input layers is dependent on the number of ANN inputs
    layerSizes[0,0] = l;                                                                                                // Input layer.
    layerSizes[1,0] = l/2;                                                                                              // First hidden layer is 1/2 the size of the input layer
    layerSizes[2,0] = l/4;                                                                                              // Second hidden layer is 1/4 the size of the input layer
    layerSizes[3,0] = catCt;                                                                                            // The output layer has one node for each category

    if( ann != NULL )
        delete ann;
    ann = new CvANN_MLP( layerSizes, CvANN_MLP::SIGMOID_SYM );

    ann->train( input, output, NULL, NULL, tParams, normFlag );

    return true;
}

void HSOM::train( int somEpochs, double initAlpha, double initR, int annIters, double annEps )
{
    random_shuffle( suspects.begin(), suspects.end() );
    try
    {
        analyzeSuspects();
        trainSOM( somEpochs, initAlpha, initR ) );
        generateHistograms();
        trainANN( annIters, annEps ) );
        statusCheck( -1 );
    }
    catch( ... )
    {
        statusCheck( -1 );
        throw;
    }
}

void HSOM::getReport( string &report, double &hitRate, double &missRate, double &nearRate )
{
    report.clear();
    hitRate = 0.0;
    missRate = 0.0;
    nearRate = 0.0;
    string hitReport, nearReport, missReport, *currReport;
    int suspectCt = suspects.size();
    int realCat, predCat;
    vector<double> votes;
    cv::Mat_<int> confusionMatrix( catCt, catCt, 0 );

    for( unsigned int i=0; i<suspects.size(); i++ )
    {
        realCat = suspects[i]->getRealCat();
        predCat = suspects[i]->getPredCat();

        if( realCat == predCat )
        {
            currReport = &hitReport;
            hitRate++;
        }
        /// @todo Review off by one logic
        else if( realCat !=0 && predCat != 0 && abs( realCat - predCat ) == 1 )
        {
            currReport = &nearReport;
            nearRate++;
        }
        else
        {
            currReport = &missReport;
            missRate++;
        }

        confusionMatrix[predCat,realCat]++;

        currReport->append( suspects[i]->getName() + " --> Class " + num2str( predCat ) + "\n" );
        if( currReport == &missReport )
        {
            votes = suspects[i]->getCatVotes();
            currReport->append( "  < " );
            for( int i=0; i<catCt; i++ )
                currReport->append( num2str( votes[i], 4 ) + " " );
            currReport->append( ">\n" );
        }
    }

    report.append( "Hits\n----\n" + hitReport );
    report.append( "\nNearMisses\n----------\n" + nearReport );
    report.append( "\nMisses\n------\n" + missReport );

    report.append( "\n\nConfusion Matrix\n----------------\n\n" );
    report.append( "              Actual\n" );
    for( int i=-2; i<catCt; i++ )
    {
        if( i < 0 )
            report.append( "             " );
        else
        {
            if( i == catCt / 2 )
                report.append( "Predicted" );
            else
                report.append( "         " );
            report.append( num2str( i, 0, 3 ) + "|" );
        }
        for( int j=0; j<catCt; j++ )
        {
            switch( i )
            {
                case -2:
                    report.append( num2str( j, 0, 5 ) );
                    break;
                case -1:
                    report.append( "-----" );
                    break;
                default:
                    report.append( num2str( confusionMatrix[i,j], 0, 5 ) );
            }
        }
        report.append( "\n" );
    }

    hitRate /= suspectCt;
    missRate /= suspectCt;
    nearRate /= suspectCt;
}

void HSOM::classify()
{
    string title = "Analyzing with " + name;
    ASSERT( statusCheck( 0, title + "__size_buffer__", title, suspects.size() ) );

    int l = grid.size().area();

    cv::Mat_<double>  input( 1, l, 0.0 );
    cv::Mat_<double> output( 1, catCt, 0.0 );

    for( unsigned int i=0; i<suspects.size(); i++ )
    {
        ASSERT( statusCheck( i++, "Classifying " + suspects[i]->getName() ) );
        classify( suspects[i], input, output );
    }
}

void HSOM::classify( Suspect* suspect, cv::Mat& input, cv::Mat& output )
{
    int l = grid.size().area();
    bool localMats = false;
    if( input.empty() )
    {                                                                                                                   // If the input and output matrix arent' specified, they need to be allocated and de-allocated in this function
        localMats = true;
        input  = cv::Mat_<double>( 1, l, 0.0 );
        output = cv::Mat_<double>( 1, catCt, 0.0 );
    }

    updateHistogram( suspect );                                                                                         // Update the histogram with features from the suspect
    suspect->setANNVectors( input );                                                                                    // Set the inputs for the back-end ANN classifier
    ann->predict( input, output );                                                                                      // Classify the suspect using the histogram and the ANN classifier
    suspect->setCatVotes( output, catCt );                                                                              // Update the vote vector for the suspect based on the output of the ANN
}

int HSOM::closestFeatureIndex( const Feature& feat )
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
            double dist = feat.dist( grid[i] );                                                                         // Calculate the distance between the input feature and the map feature at index i
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

PointPlus<int> HSOM::closestFeatureCoords( const Feature& feat )
{
    return grid.getCoords( closestFeatureIndex( feat ) );                                                               // Use the protected closestFeature to find the index and translate it into coordinates
}

void HSOM::preCalcWeights( const double alpha, const double radius )
{
    double sigma = radius / FWHM_FACTOR;                                                                                // The radius will be used to describe the Full Width at Half Maximum of the gaussian weighting function.  We back-compute sigma from this.
    double twoSigmaSquared = 2 * pow( sigma, 2 );                                                                       // Value of 2 * sigma^2
    weights = vector<double>( (int)radius + 1, 0.0 );
    for( unsigned int i=0; i<weights.size(); i++ )
        weights[i] = alpha * exp( -pow( i, 2.0 ) / twoSigmaSquared );                                                   // Calculate the gaussian weighting function.  This function is dependant on alpha, sigma, and current distance from origin.
}

void HSOM::updateSOM( const Suspect& suspect )
{
    int dist, idx, x, y;
    vector<int> neighbors;
    Feature& feat = suspect.getNextFeature();
    for( int i=0; i<suspect.featureCt(); i++ )
    {
        Feature& feat = suspect[i];
    }
    while( feat != NULL ){                                                                                              // While the suspect has features to provide
        idx = closestFeature( feat );                                                                                   // Find the feature that is closest to the training feature
        getCoords( idx, x, y );                                                                                         // Get the coordinates of the closest feature
        neighbors = getNeighborhood( (double)weights.size(), x, y );                                                    // Get all indices of features within the neighborhood radius.  The distances are pre-calculated and stored in the values slot

#pragma omp parallel private( idx, dist )
        { /* +omp */
#pragma omp for nowait
        for( int i=0; i<(int)neighbors.size(); i++ ){                                                                   // Iterate over all features in the neighborhood and update them to make them more similar to the training feature.
            idx = neighbors[i];
            dist = (int)values[idx];                                                                                    // Get the precalculated distance
            features[idx]->adjust( feat, weights[dist] );                                                               // Adjust the SOM feature using the precalculated weight
        }
        } /* -omp */

        delete feat;
        feat = suspect->getNextFeature();                                                                               // Get the next feature in the suspect
    }
    RETURN;
}

void HSOM::updateHistogram( Suspect* suspect ){
    ENTER;
    int idx;
    Feature* feat = suspect->getNextFeature();                                                                          // Fetch the first feature from the suspect
    while( feat != NULL ){                                                                                              // While the suspect still has features to count,
        idx = closestFeature( feat );                                                                                   // Find the feature that is closest to the training feature
        suspect->incrementHistogram( idx );                                                                             // Increment the corresponding historam bin
        delete feat;
        feat = suspect->getNextFeature();                                                                               // Get the next feature in the suspect
    }
    RETURN;
}

string HSOM::getName(){
    ENTER;
    RETURN name;
}

int HSOM::getCatCt(){
    ENTER;
    RETURN catCt;
}

void HSOM::save( string fileName ){
    ENTER;
    CvFileStorage* fs = cvOpenFileStorage( fileName.c_str(), NULL, CV_STORAGE_WRITE );
    name = fileName;
    write( fs );
    cvReleaseFileStorage( &fs );
}

void HSOM::load( string fileName ){
    ENTER;
    CvFileStorage* fs = cvOpenFileStorage( fileName.c_str(), NULL, CV_STORAGE_READ );
    name = fileName;
    read( fs );
    cvReleaseFileStorage( &fs );
}

void HSOM::read( CvFileStorage* fs, CvFileNode* node ){
    ENTER;
    clear();
    node = cvGetFileNodeByName( fs, node, HSOM_ALIAS );
    catCt = cvReadIntByName( fs, node, "catCt" );
    w = cvReadIntByName( fs, node, "w" );
    h = cvReadIntByName( fs, node, "h" );
    setSize( w, h );
    initFeatures();
    CvFileNode* feat_root = cvGetFileNodeByName( fs, node, "feature_vector" );
    CvFileNode* feat_node;
    l = cvReadIntByName( fs, feat_root, "l" );
    string feat_name;
    for( int i=0; i<l; i++ ){
        feat_name = "feature" + num2str(i);
        feat_node = cvGetFileNodeByName( fs, feat_root, feat_name.c_str() );
        features[i]->read( fs, feat_node );
    }
    CvFileNode* ann_node = cvGetFileNodeByName( fs, node, "ann" );
    if( ann == NULL )
        ann = new CvANN_MLP();
    ann->read( fs, ann_node );
    RETURN;
}

void HSOM::write( CvFileStorage* fs ){
    ENTER;
    cvStartWriteStruct( fs, HSOM_ALIAS, CV_NODE_MAP );
        cvWriteInt( fs, "catCt", catCt );
        cvWriteInt( fs, "w", w );
        cvWriteInt( fs, "h", h );
        cvStartWriteStruct( fs, "feature_vector", CV_NODE_MAP );
            cvWriteInt( fs, "l", l );
            string feat_name;
            for( int i=0; i<l; i++ ){
                feat_name = "feature" + num2str(i);
                cvStartWriteStruct( fs, feat_name.c_str(), CV_NODE_MAP );
                features[i]->write( fs );
                cvEndWriteStruct( fs );
            }
        cvEndWriteStruct( fs );
        ann->write( fs, "ann" );
    cvEndWriteStruct( fs );
    RETURN;
}
