#include "annclassifier.h"

namespace somtk {

ANNClassifier::ANNClassifier( QString fileName ) :
    ann( NULL ),
    fileName( fileName )
{}



void ANNClassifier::readClassifierData( QDomElement& element )
{
    fileName = element.attribute( "fileName" );

    if( ann != NULL )
        delete ann;
    ann = new CvANN_MLP();
    ann->load( fileName.toAscii() );

    /// @todo  Insert some sort of verification here to make sure that the load went ok
}



void ANNClassifier::writeClassifierData( QDomElement& element )
{
    element.setAttribute( "fileName", fileName );

    if( ann != NULL )
        ann->save( fileName.toAscii() );
}



void ANNClassifier::trainClassifier( QVector<SuspectPtr> suspects, QMap<QString,QVariant> trainingParameters )
{
    bool ok = true;

    /// @todo  Move the input and output width parameters down into the base class
    // Fetch the input width for this classifier
    ASSERT_MSG( trainingParameters.contains( "inputWidth" ), "Missing required input width parameter" );
    int inputWidth = trainingParameters[ "inputWidth" ].toInt( &ok );
    ASSERT_MSG( ok, "Failed to convert input width parameter" );
    ASSERT_MSG( inputWidth > 0, "Invalid input size" );

    // Fetch the output width for this classifier
    ASSERT_MSG( trainingParameters.contains( "outputWidth" ), "Missing required output width parameter" );
    int outputWidth = trainingParameters[ "outputWidth" ].toInt( &ok );
    ASSERT_MSG( ok, "Failed to convert output width parameter" );
    ASSERT_MSG( outputWidth > 0, "Invalid output size" );

    // Fetch the iterations parameter for this classifier
    int iterations = 0;
    if( trainingParameters.contains( "iterations" ) )
    {
        iterations = trainingParameters[ "iterations" ].toInt( &ok );
        ASSERT_MSG( ok, "Failed to convert iterations parameter" );
        ASSERT_MSG( iterations > 0, "Invalid iterations value" );
    }

    // Fetch the iterations parameter for this classifier
    double epsilon = 0.0;
    if( trainingParameters.contains( "epsilon" ) )
    {
        iterations = trainingParameters[ "epsilon" ].toDouble( &ok );
        ASSERT_MSG( ok, "Failed to convert epsilon parameter" );
        ASSERT_MSG( epsilon > 0.0, "Invalid epsilon value" );
    }

    /* Make an input matrix for training the back-end MLPANN.
     * This is a matrix composed of vectors the length of the SOM's area ( width x height )
     */
    cv::Mat_<double>  input( suspects.size(), inputWidth, 0.0 );

    /* Make an output matrix for trainig the back-end MLPANN.
     * This is a matrix composed of vectors the length of the number of classification categories.
     */
    cv::Mat_<double> output( suspects.size(), outputWidth, 0.0 );

    for( int i=0; i<suspects.size(); i++ )
    {
        SuspectPtr suspect = suspects[i];
        HistogramPtr histogram = suspect->histogram();

        // Set the input for the training input matrix row to the histogram of the suspect
        for( int j=0; j<inputWidth; j++ )
            input( i, j ) = histogram[j];

        // Set the output value for the real category of the suspect to 1.  All other values will already be 0
        output( i, suspect->realCategory() ) = 1.0;
    }

    cv::TermCriteria terminationCriteria;
    terminationCriteria.type = 0;
    if( iterations != 0 )
        terminationCriteria.type += cv::TermCriteria::MAX_ITER;
    if( epsilon != 0.0 )
        terminationCriteria.type += cv::TermCriteria::EPS;

    cv::ANN_MLP_TrainParams trainingParams( terminationCriteria, CvANN_MLP_TrainParams::RPROP, iterations, epsilon );

    int normFlag = 0;

    int tmp[4];

    // The input layer has one node for each element in the input histograms
    tmp[0] = inputWidth;

    // The first hidden layer is half the size of the input layer
    tmp[1] = inputWidth / 2;

    // The second input layer is one quarter the size of the input layer
    tmp[2] = inputWidth / 4;

    // The output layer has one node for each category
    tmp[3] = outputWidth;

    CvMat ls = cvMat( 4, 1, CV_32S, tmp );

    // If there is already an MPLANN attached to this HSOM, delete it to start over
    if( _ann != NULL )
        delete _ann;

    // Create a new MLPANN for the HSOM
    _ann = new CvANN_MLP( &ls, CvANN_MLP::SIGMOID_SYM );

    _ann->train( input, output, cv::Mat(), cv::Mat(), trainingParams, normFlag );
}

void ANNClassifier::classify( SuspectPtr suspect )
{
    bool ok = true;

    /// @todo  Move the input and output width parameters down into the base class
    // Fetch the input width for this classifier
    ASSERT_MSG( trainingParameters.contains( "inputWidth" ), "Missing required input width parameter" );
    int inputWidth = trainingParameters[ "inputWidth" ].toInt( &ok );
    ASSERT_MSG( ok, "Failed to convert input width parameter" );
    ASSERT_MSG( inputWidth > 0, "Invalid input size" );

    // Fetch the output width for this classifier
    ASSERT_MSG( trainingParameters.contains( "outputWidth" ), "Missing required output width parameter" );
    int outputWidth = trainingParameters[ "outputWidth" ].toInt( &ok );
    ASSERT_MSG( ok, "Failed to convert output width parameter" );
    ASSERT_MSG( outputWidth > 0, "Invalid output size" );

    cv::Mat_<double>  input( 1, inputWidth, 0.0 );
    cv::Mat_<double> output( 1, outputWidth, 0.0 );
    /// @todo  ensure that the output widths match

    HistogramPtr histogram = suspect->histogram();
    for( int i=0; i<histogram->size(); i++ )
        input[i] = histogram[i];

    ann->predict( input, output );

    QVector<double> classification;
    for( int i=0; i<Suspect::categoryCount(); i++ )
        classification.push_back( output[i] );

    suspect->setClassification( classification );
}

} // namespace