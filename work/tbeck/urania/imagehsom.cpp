#include "imagehsom.h"

using namespace std;

const string ImageHSOM::alias = "ImageHSOM";

ImageHSOM::ImageHSOM() : HSOM(){}

ImageHSOM::ImageHSOM( const SizePlus<int>& sz, const SizePlus<int>& featSz, int catCt )
    : HSOM( sz, catCt ), _featSz(featSz)
{
    initFeatures();
}

ImageHSOM::ImageHSOM( string fileName ) : HSOM()
{
    load( fileName );
}

ImageHSOM::ImageHSOM( cv::FileNode& fn ) : HSOM()
{
    read( fn );
}

ImageHSOM::~ImageHSOM(){}

void ImageHSOM::featSz()
{
    return _featSz;
}

void ImageHSOM::initFeatures()
{
    for( int i=0; i<l; i++ )
        grid[i] = new HuFeature();
}

/// @todo  determine if this function is necessary
ImageSuspect* ImageHSOM::createSuspect( const cv::Mat& img, const cv::Mat& msk, int realCalt, string name )
{
    return new ImageSuspect( img, msk, realCat, catCt, w, h, featW, featH, name );
}

cv::Mat loadImage64f( string fileName, const SizePlus<int>& imgSz )
{
    cv::Mat src = cv::imread( fileName, 0 );
    ASSERT_MSG( !src.empty(), "Couldn't load source image file." );
    cv::Mat dst = cvtTo64f1c( src, imgSz);
    return dst;
}

cv::Mat cvtTo8u1c( const cv::Mat& src, const SizePlus<int>& imgSz )
{
    SizePlus<int> dstSz;
    dstSz.w = imgSz.w == 0 ? src.size().width  : imgSz.w;
    dstSz.h = imgSz.h == 0 ? src.size().height : imgSz.h;

    cv::Mat::type()
    cv::Mat tmp1( src.size(), src.depth(), )
    IplImage* tmp1 = cvCreateImage( IPL_SZ(src), src->depth, 1 );
    IplImage* tmp2 = cvCreateImage( IPL_SZ(src), IPL_DEPTH_8U, 1 );
    IplImage* dst  = cvCreateImage( cvSize(imgW,imgH), IPL_DEPTH_8U, 1 );
    switch( src->nChannels ){
        case 4:
        cvCvtColor( src, tmp1, CV_BGRA2GRAY );
        break;
        case 3:
        cvCvtColor( src, tmp1, CV_BGR2GRAY );
        break;
        case 1:
        cvCopy( src, tmp1 );
        break;
    }
    cvNormalize( tmp1, tmp1, 0, 255, CV_MINMAX );
    cvCvtScale( tmp1, tmp2 );
    cvResize( tmp2, dst, CV_INTER_CUBIC );                                                                              // Note that resizing has to be done after scale conversion because OpenCV doesn't know how to resize a floating point image
    cvReleaseImage( &tmp1 );
    cvReleaseImage( &tmp2 );
    RETURN dst;
}

IplImage* cvtTo64f1c( IplImage* src, int imgW, int imgH ){
    ENTER;
    if( imgW == 0 )
        imgW = src->width;
    if( imgH == 0 )
        imgH = src->height;
    IplImage* tmp1 = cvCreateImage( cvSize(imgW,imgH), src->depth, src->nChannels );
    IplImage* tmp2 = cvCreateImage( cvSize(imgW,imgH), src->depth, 1 );
    IplImage* dst = cvCreateImage( cvSize(imgW,imgH), IPL_DEPTH_64F, 1 );
    cvResize( src, tmp1, CV_INTER_CUBIC );
    switch( src->nChannels ){
        case 4:
        cvCvtColor( tmp1, tmp2, CV_BGRA2GRAY );
        break;
        case 3:
        cvCvtColor( tmp1, tmp2, CV_BGR2GRAY );
        break;
        case 1:
        cvCopy( tmp1, tmp2 );
        break;
    }
    cvCvtScale( tmp2, dst );
    cvNormalize( dst, dst, 0, 1, CV_MINMAX );
    cvReleaseImage( &tmp1 );
    cvReleaseImage( &tmp2 );
    RETURN dst;
}

void ImageHSOM::loadSuspects( string dirPath, const vector<string> &fileList )
{
    clearSuspects();
    string fileName;
    int suspectCt = fileList.size();
    int realCat;
    cv::Mat img, msk;
    ASSERT( statusCheck( 0, "Loading Suspects", "Loading Suspects", suspectCt ) );
    for( int i=0; i<suspectCt; i++ )
    {
        ASSERT( statusCheck( i, "Loading Suspect " + num2str(i) ) );
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
