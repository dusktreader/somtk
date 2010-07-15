#include "imagehsom.h"
#include <time.h>

using namespace std;

ImageHSOM::ImageHSOM() : HSOM(){
    ENTER;
    featW = -1;
    featH = -1;
    RETURN;
}

ImageHSOM::ImageHSOM( int w, int h, int featW, int featH, int catCt ) : HSOM( w, h, catCt ) {
    ENTER;
    ASSERT_SET( this->featW, featW, featW > 0, "Feature width must have a positive size" );
    ASSERT_SET( this->featH, featH, featH > 0, "Feature height must have a positive size" );
    initFeatures();
    RETURN;
}

ImageHSOM::ImageHSOM( string fileName ) : HSOM(){
    ENTER;
    load( fileName );
    RETURN;
}

ImageHSOM::ImageHSOM( CvFileStorage* fs, CvFileNode* node ) : HSOM(){
    ENTER;
    read( fs, node );
    RETURN;
}

ImageHSOM::~ImageHSOM(){
    ENTER;
    RETURN;
}

void ImageHSOM::getFeatSize( int &featW, int &featH ){
    ENTER;
    featW = this->featW;
    featH = this->featH;
    RETURN;
}

void ImageHSOM::initFeatures(){
    ENTER;
    for( int i=0; i<l; i++ )
        features.push_back( new HuFeature() );
    RETURN;
}

void ImageHSOM::AddData( ThumbData &input ){
    ENTER;
    IplImage img = (IplImage)input.original;
    IplImage msk = (IplImage)input.simple;
    int realCat = input.response;
    string name = input.filename;
    suspects.push_back( createSuspect( &img, &msk, realCat, name ) );
    RETURN;
}

void ImageHSOM::Train(){
    ENTER;
    train( 10, 0.2, 0.3, 0, 0.0 );
    RETURN;
}

void ImageHSOM::Read( CvFileStorage* fs, CvFileNode* fsNode ){
    ENTER;
    read( fs, fsNode );
    RETURN;
}

void ImageHSOM::Write( CvFileStorage* fs ){
    ENTER;
    write( fs );
    RETURN;
}

vector<int> ImageHSOM::PredictCategorical( ThumbData &input ){
    ENTER;
    IplImage img = (IplImage)input.original;
    IplImage msk = (IplImage)input.simple;
    int realCat = input.response;
    string name = input.filename;
    Suspect* suspect = createSuspect( &img, &msk, realCat, name );
    classify( suspect );
    vector<int> predVect( 1, suspect->getPredCat() );
    delete suspect;
    RETURN predVect;
}

ImageSuspect* ImageHSOM::createSuspect( IplImage* img, IplImage* msk, int realCat, string name ){
    ENTER;
    ImageSuspect* suspect = new ImageSuspect( img, msk, realCat, catCt, w, h, featW, featH, name );
    RETURN suspect;
}

bool ImageHSOM::loadSuspects( string dirPath, const vector<string> &fileList ){
    ENTER;
    clearSuspects();
    string fileName;
    int suspectCt = fileList.size();
    int realCat;
    IplImage* img;
    IplImage* msk;
    bool alive = statusCheck( 0, "Loading Suspects",
                              "Loading Suspects", suspectCt );
    for( int i=0; i<suspectCt; i++ ){
        alive = statusCheck( i, "Loading Suspect " + num2str(i) );
        if( !alive )
            break;
        fileName = dirPath + OS_SEP_STR + fileList[i];
        img = loadImage64f( fileName, 128, 128 );
        msk = starMask( img );
        realCat = char2Int( fileList[i][0] );
        suspects.push_back( createSuspect( img, msk, realCat, fileList[i] ) );
        cvReleaseImage( &img );
        cvReleaseImage( &msk );
    }
    RETURN alive;
}

bool ImageHSOM::analyzeSuspects(){
    int max_feats = 100000;
    int featCt = 0;
    vector<double> huArr;
    vector<double> hus;
    vector<int> indices;
    HuFeature* feat;

    bool alive = statusCheck( 0, "Analyzing suspect Hu values",
                              "Analyzing suspect Hu values", suspects.size() );
    int i = 0;
    for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ ){                                     // Iterate over all training suspects
        alive = statusCheck( i++, "Analyzing Suspect " + (*s)->getName() );
        if( !alive )
            break;

        if( featCt < max_feats ){
            feat = (HuFeature*)((*s)->getNextFeature());
            while( feat != NULL && featCt < max_feats ){
                hus = feat->getOriginalHus();
                huArr.insert( huArr.end(), hus.begin(), hus.end() );
                featCt++;
                delete feat;
                feat = (HuFeature*)((*s)->getNextFeature());
            }
        }
    }

    if( !alive ){
        RETURN false;
    }

    cv::Scalar meanSclr;
    cv::Scalar stdvSclr;
    vector<double> huMean(  7, 0.0 );
    vector<double> huStdv(  7, 0.0 );
    vector<double> huAlpha( 7, 0.0 );
    double eps = 0.0625;                                                                                                /// @todo  these should probably be parameterized....
    int sigmaStep = 2;

    Mat huMat( huArr, false );
    huMat.rows = featCt;
    huMat.cols = 7;
    Mat huCol;

    for( int j=0; j<7; j++ ){
        huCol = huMat.col( j );
        cv::meanStdDev( huCol, meanSclr, stdvSclr );
        huMean[j] = meanSclr[0];
        huStdv[j] = stdvSclr[0];
        huAlpha[j] = log( 1 / eps - 1 ) / ( sigmaStep * huStdv[j] );
    }

    HuFeature::setSigmoidParams( huMean, huStdv, huAlpha );

    RETURN alive;
}

void ImageHSOM::write( CvFileStorage* fs ){
    ENTER;
    double* tdata = new double[7];
    cvStartWriteStruct( fs, IMAGE_HSOM_ALIAS, CV_NODE_MAP );
        cvWriteInt( fs, "featW", featW );
        cvWriteInt( fs, "featH", featH );

        cvStartWriteStruct( fs, "sigmoid_parameters", CV_NODE_MAP );
            vector<double> huMean(  7, 0.0 );
            vector<double> huStdv(  7, 0.0 );
            vector<double> huAlpha( 7, 0.0 );
            HuFeature::getSigmoidParams( huMean, huStdv, huAlpha );
            cvStartWriteStruct( fs, "hu_mean_values", CV_NODE_SEQ );
                copy( huMean.begin(), huMean.end(), tdata );
                cvWriteRawData( fs, tdata, 7, "d" );
            cvEndWriteStruct( fs );

            cvStartWriteStruct( fs, "hu_standard_deviations", CV_NODE_SEQ );
                copy( huStdv.begin(), huStdv.end(), tdata );
                cvWriteRawData( fs, tdata, 7, "d" );
            cvEndWriteStruct( fs );

            cvStartWriteStruct( fs, "hu_alpha_values", CV_NODE_SEQ );
                copy( huAlpha.begin(), huAlpha.end(), tdata );
                cvWriteRawData( fs, tdata, 7, "d" );
            cvEndWriteStruct( fs );

        cvEndWriteStruct( fs );

        HSOM::write( fs );

    cvEndWriteStruct( fs );
    delete [] tdata;
    RETURN;
}

void ImageHSOM::read( CvFileStorage* fs, CvFileNode* node ){
    ENTER;
    double* tdata = new double[7];
    node = cvGetFileNodeByName( fs, node, IMAGE_HSOM_ALIAS );
    featW = cvReadIntByName( fs, node, "featW" );
    featH = cvReadIntByName( fs, node, "featH" );

    CvFileNode* hu_root = cvGetFileNodeByName( fs, node, "sigmoid_parameters" );

    CvFileNode* hu_node = cvGetFileNodeByName( fs, hu_root, "hu_mean_values" );
    cvReadRawData( fs, hu_node, tdata, "d" );
    vector<double> huMean( tdata, tdata+7 );

    hu_node = cvGetFileNodeByName( fs, hu_root, "hu_standard_deviations" );
    cvReadRawData( fs, hu_node, tdata, "d" );
    vector<double> huStdv( tdata, tdata+7 );

    hu_node = cvGetFileNodeByName( fs, hu_root, "hu_alpha_values" );
    cvReadRawData( fs, hu_node, tdata, "d" );
    vector<double> huAlpha( tdata, tdata+7 );

    HuFeature::setSigmoidParams( huMean, huStdv, huAlpha );

    HSOM::read( fs, node );
    delete [] tdata;
    //DB_REP;
    RETURN;
}
