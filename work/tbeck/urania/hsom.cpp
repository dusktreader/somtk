#include "hsom.h"

using namespace std;

HSOM::HSOM() : HexGrid(){
    ENTER;
    catCt = -1;
    ann = NULL;
    RETURN;
}

HSOM::HSOM( int w, int h, int catCt ) : HexGrid( w, h ){
    ENTER;
    this->catCt = catCt;
    ann = NULL;
    name = "unnamed";
    RETURN;
}

HSOM::~HSOM(){
    ENTER;
    clear();
    delete ann;
    RETURN;
}

void HSOM::clear(){
    ENTER;
    clearFeatures();
    clearSuspects();
    HexGrid::clear();
    RETURN;
}

void HSOM::clearFeatures(){
    ENTER;
    for( vector<Feature*>::iterator f = features.begin(); f != features.end(); f++ )
        delete *f;
    features.clear();
    RETURN;
}

void HSOM::clearSuspects(){
    ENTER;
    for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ )
        delete *s;
    suspects.clear();
    RETURN;
}

bool HSOM::statusCheck( int iteration, string msg1, string msg2, int maxIters ){
    ENTER;
    cout << "HSOM training " << iteration << "/" << maxIters << ": " << msg1 << " ... " << msg2 << endl;
    RETURN true;
}

bool HSOM::analyzeSuspects(){
    ENTER;
    RETURN true;
}

bool HSOM::trainSOM( int somEpochs, double initAlpha, double initRadRat ){
    ENTER;
    ASSERT_MSG( initRadRat <= 0.5, "Inital radius ratio may not exceed 1/2" );

    bool alive = statusCheck( 0, "Beginning SOM training...", "Training SOM" );
    if( !alive ){
        RETURN false;
    }

    double radius;                                                                                                      /// @keyword  ts1253137089  @keyword  ts1247868419 - Now defunct: replaced with exponential decay
    double radius0 = initRadRat * w;                                                                                    /// @todo  Make radius_tf, radius_Nf, alpha_tf, alpha_Nf arguments to this function
    double radius_tf = 0.25;
    double radius_Nf = 0.5;
    double radius_gamma = -log( radius_Nf ) / ( radius_tf * somEpochs );

    double alpha;
    double alpha0 = initAlpha;
    double alpha_tf = 0.1;
    double alpha_Nf = 0.25;
    double alpha_gamma = -log( alpha_Nf ) / ( alpha_tf * somEpochs );

    for( int E=0; E<somEpochs; E++ ){

        alive = statusCheck( 0, "---", "SOM Epoch " + num2str( E + 1 ) + "/" + num2str( somEpochs ) );
        if( !alive ){
            RETURN false;
        }

        random_shuffle( suspects.begin(), suspects.end() );                                                             // Shuffle the list of suspects so that representatives from each category appear uniformly throught the training

        alpha = alpha0 * exp( -alpha_gamma * E );
        radius = radius0 * exp( -radius_gamma * E );
        preCalcWeights( alpha, radius );                                                                                // Pre-calculate the gaussian weights for updating the SOM
        int i = 0;
        for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ ){                                 // Iterate over all training suspects
            alive = statusCheck( i++, "Processing " + (*s)->getName() );
            if( !alive ){
                RETURN false;
            }
            updateSOM( *s );
        }
    }
    RETURN true;
}

bool HSOM::generateHistograms(){
    ENTER;
    bool alive = statusCheck( 0, "---", "Generating histograms" );
    if( !alive ){
        RETURN false;
    }

    int i = 0;
    for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ ){                                     // Iterate over all training suspects
        alive = statusCheck( i++, "Processing " + (*s)->getName() );
        if( !alive ){
            RETURN false;
        }
        updateHistogram( *s );                                                                                          // Update each suspects's histogram now that the SOM part is fully trained.
    }
    RETURN true;
}

bool HSOM::trainANN( int annIters, double annEps ){
    ENTER;
    bool alive = statusCheck( 0, "---", "Training ANN" );
    if( !alive ){
        RETURN false;
    }

    CvMat* input = cvCreateMat( suspects.size(), l, CV_64F );                                                           // Make an input matrix for training the back-end MLPANN.  This is a matrix composed of vectors of size l = SOM_W * SOM_H
    CvMat* output = cvCreateMat( suspects.size(), catCt, CV_64F );                                                      // Make an output matrix for trainig the back-end MLPANN.  This is a matrix composed of vectors of size catCt.
    double* inPtr = input->data.db;                                                                                     // These two utility pointers will point to the current locations within the input and output matrices
    double* outPtr = output->data.db;


    int i = 0;
    for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ ){                                     // Iterate over all training suspects
        alive = statusCheck( i++, "Extracting ANN inputs from " + (*s)->getName() );
        if( !alive ){
            cvReleaseMat( &input );
            cvReleaseMat( &output );
            RETURN false;
        }
        (*s)->setANNVectors( inPtr, l, outPtr, catCt );                                                                 // Update the input and output matrices with values from the suspect's histogram
        inPtr += l;
        outPtr += catCt;
    }

    int tcType = 0;
    if( annIters != 0 )
        tcType += CV_TERMCRIT_ITER;
    if( annEps != 0.0 )
        tcType += CV_TERMCRIT_EPS;

    CvANN_MLP_TrainParams tParams;
    tParams.term_crit = cvTermCriteria( tcType, annIters, annEps );
    tParams.train_method = CvANN_MLP_TrainParams::RPROP;

    int normFlag = 0;

    CvMat* layerSizes = cvCreateMat( 4, 1, CV_32S );                                                                    /// @todo  Make this more dynamic, so that the size and count of input layers is dependent on the number of ANN inputs
    layerSizes->data.i[0] = l;                                                                                          // Input layer.
    layerSizes->data.i[1] = l/2;                                                                                        // First hidden layer is 1/2 the size of the input layer
    layerSizes->data.i[2] = l/4;                                                                                        // Second hidden layer is 1/4 the size of the input layer
    layerSizes->data.i[3] = catCt;                                                                                      // The output layer has one node for each category

    if( ann == NULL )
        delete ann;
    ann = new CvANN_MLP( layerSizes, CvANN_MLP::SIGMOID_SYM );

    ann->train( input, output, NULL, NULL, tParams, normFlag );

    cvReleaseMat( &input );
    cvReleaseMat( &output );
    cvReleaseMat( &layerSizes );

    RETURN true;
}

bool HSOM::train( int somEpochs, double initAlpha, double initR, int annIters, double annEps ){
    ENTER;
    random_shuffle( suspects.begin(), suspects.end() );
    if( !analyzeSuspects() ){
        RETURN false;
    }
    if( !trainSOM( somEpochs, initAlpha, initR ) ){
        RETURN false;
    }
    if( !generateHistograms() ){
        RETURN false;
    }
    if( !trainANN( annIters, annEps ) ){
        RETURN false;
    }
    statusCheck( -1 );
    RETURN true;
}

void HSOM::getReport( string &report, double &hitRate, double &missRate, double &nearRate ){
    ENTER;
    report.clear();
    hitRate = 0.0;
    missRate = 0.0;
    nearRate = 0.0;
    string hitReport, nearReport, missReport, *currReport;
    int suspectCt = suspects.size();
    int realCat, predCat;
    vector<double> votes;
    int* confusionMatrix= new int[ catCt * catCt ];
    for( int i=0; i<catCt; i++ )
        for( int j=0; j<catCt; j++ )
            confusionMatrix[ i * catCt + j ] = 0;

    for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ ){                                            // Iterate over all suspects

        realCat = (*s)->getRealCat();
        predCat = (*s)->getPredCat();

        if( realCat == predCat ){
            currReport = &hitReport;
            hitRate++;
        }
        else if( realCat !=0 && predCat != 0 && abs( realCat - predCat ) == 1 ){
            currReport = &nearReport;
            nearRate++;
        }
        else{
            currReport = &missReport;
            missRate++;
        }

        confusionMatrix[ predCat * catCt + realCat ]++;

        currReport->append( (*s)->getName() + " --> Class " + num2str( predCat ) + "\n" );
        if( currReport == &missReport ){
            votes = (*s)->getCatVotes();
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
    for( int i=-2; i<catCt; i++ ){
        if( i < 0 )
            report.append( "             " );
        else{
            if( i == catCt / 2 )
                report.append( "Predicted" );
            else
                report.append( "         " );
            report.append( num2str( i, 0, 3 ) + "|" );
        }
        for( int j=0; j<catCt; j++ ){
            switch( i ){
                case -2:
                    report.append( num2str( j, 0, 5 ) );
                    break;
                case -1:
                    report.append( "-----" );
                    break;
                default:
                    report.append( num2str( confusionMatrix[i*catCt+j], 0, 5 ) );
            }
        }
        report.append( "\n" );
    }

    hitRate /= suspectCt;
    missRate /= suspectCt;
    nearRate /= suspectCt;
    delete [] confusionMatrix;
    RETURN;
}

bool HSOM::classify(){
    ENTER;
    string title = "Analyzing with " + name;
    bool alive = statusCheck( 0, title + "__size_buffer__", title, suspects.size() );
    if( !alive ){
        RETURN false;
    }

    CvMat* input = cvCreateMat( 1, l, CV_64F );                                                                         // Alocate an input an output matrix for classifying the suspects with the back-end ANN
    CvMat* output = cvCreateMat( 1, catCt, CV_64F );

    int i = 0;
    for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ ){                                            // Iterate over all training suspects
        alive = statusCheck( i++, "Classifying " + (*s)->getName() );
        if( !alive )
            break;
        classify( *s, input, output );
    }

    cvReleaseMat( &input );
    cvReleaseMat( &output );

    RETURN alive;
}

void HSOM::classify( Suspect* suspect, CvMat* input, CvMat* output ){
    ENTER;
    bool localMats = false;
    if( input == NULL ){                                                                                                // If the input and output matrix arent' specified, they need to be allocated and de-allocated in this function
        localMats = true;
        input = cvCreateMat( 1, l, CV_64F );
        output = cvCreateMat( 1, catCt, CV_64F );
    }

    updateHistogram( suspect );                                                                                         // Update the histogram with features from the suspect
    suspect->setANNVectors( input->data.db, l );                                                                        // Set the inputs for the back-end ANN classifier
    ann->predict( input, output );                                                                                      // Classify the suspect using the histogram and the ANN classifier
    suspect->setCatVotes( output->data.db, catCt );                                                                     // Update the vote vector for the suspect based on the output of the ANN

    if( localMats ){                                                                                                    // If the input and output matrices are allocated locally, de-allocate them
        cvReleaseMat( &input );
        cvReleaseMat( &output );
    }
    RETURN;
}

int HSOM::closestFeature( Feature* feat ){
    ENTER;
    int closestIndex = 0;                                                                                               // Initialize the search by setting the 0th feature to be the closest
    double minDist = feat->dist( features[0] );
    double currDist;
    int i;
#pragma omp parallel shared( closestIndex, minDist ) private( i, currDist  )                                            /// @keyword ts1247874477
    { /* +omp */
#pragma omp for nowait
    for( i=1; i<l; i++ ){                                                                                               // Search over all features
        currDist = feat->dist( features[i] );                                                                           // Calculate the distance between the input feature and the map feature at index i
#pragma omp critical
        { /* +omp */
        if( currDist < minDist ){                                                                                       // If the current feature is closer than the closest feature so far, make the current feature the closest feature
            closestIndex = i;
            minDist = currDist;
        }
        } /* -omp */
    }
    } /* -omp */
    RETURN closestIndex;
}

void HSOM::closestFeature( Feature* feat, int &x, int &y ){
    ENTER;
    getCoords( closestFeature( feat ), x, y );                                                                          // Use the protected closestFeature to find the index and translate it into coordinates
    RETURN;
}

void HSOM::preCalcWeights( const double alpha, const double radius ){
    ENTER;
    double sigma = radius / FWHM_FACTOR;                                                                                // The radius will be used to describe the Full Width at Half Maximum of the gaussian weighting function.  We back-compute sigma from this.
    double twoSigmaSquared = 2 * pow( sigma, 2 );                                                                       // Value of 2 * sigma^2
    weights.clear();
    weights = vector<double>( (int)radius + 1, 0.0 );
    for( int i=0; i<(int)weights.size(); i++ )
        weights[i] = alpha * exp( -pow( i, 2.0 ) / twoSigmaSquared );                                                   // Calculate the gaussian weighting function.  This function is dependant on alpha, sigma, and current distance from origin.
    RETURN;
}

void HSOM::updateSOM( Suspect* suspect ){
    ENTER;
    int dist, idx, x, y;
    vector<int> neighbors;
    Feature* feat = suspect->getNextFeature();                                                                          // Begin extracting features from the suspect
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