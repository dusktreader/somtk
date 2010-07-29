#include "qimagehsom.h"

using namespace std;

QImageHSOM::QImageHSOM( QProgressDialog* pgrs ) : ImageHSOM(), pgrs(pgrs){}

QImageHSOM::QImageHSOM( QProgressDialog* pgrs,
                        SizePlus<int> gridSz, SizePlus<int> featSz, int stepSz,
                        int catCt ) :
ImageHSOM( gridSz, featSz, stepSz, catCt ), pgrs(pgrs){}

QImageHSOM::QImageHSOM( QProgressDialog* pgrs, QString fileName ) :
ImageHSOM( fileName.toStdString() ), pgrs(pgrs){}

QImageHSOM::~QImageHSOM(){}

void QImageHSOM::loadSuspects( QString qDirPath, const QStringList &qFileList )
{
    string dirPath = qDirPath.toStdString();
    vector<string> fileList;
    for( int i=0; i<qFileList.length(); i++ )
        fileList.push_back( qFileList.at(i).toStdString() );
    ImageHSOM::loadSuspects( dirPath, fileList );
}

bool QImageHSOM::statusCheck( int iteration, string msg1, string msg2, int maxIters )
{
    if( pgrs == NULL )
        return true;
    if( pgrs->wasCanceled() == true )
    {
        pgrs->reset();
        return false;
    }
    if( maxIters != 0 )
    {
        pgrs->setMinimum( 0 );
        pgrs->setMaximum( maxIters );
        pgrs->setLabelText( "-----------------------------------------------" );
        pgrs->setMinimumDuration( 0 );
    }
    pgrs->setValue( iteration );
    pgrs->setLabelText( QString::fromStdString( msg1 ) );
    if( msg2 != "" )
        pgrs->setWindowTitle( QString::fromStdString( msg2 ) );
    pgrs->update();

    return true;
}

QPixmap iplToQPixmap( const cv::Mat& src, const QSize& dstSz, Qt::TransformationMode tmode, Qt::AspectRatioMode amode )
{
    cv::Mat tmp()

    IplImage* tmp = cvCreateImage( IPL_SZ( src ), IPL_DEPTH_8U, src->nChannels );
    IplImage* dst = cvCreateImage( IPL_SZ( src ), IPL_DEPTH_8U, 3 );


    switch( src->depth ){
        case IPL_DEPTH_8U:
        case IPL_DEPTH_8S:
        case IPL_DEPTH_16U:
        case IPL_DEPTH_16S:
        cvCvtScale( src, tmp, 1.0, 0.0 );
        break;
        case IPL_DEPTH_32F:
        case IPL_DEPTH_64F:
        cvCvtScale( src, tmp, 255.0, 0.0 );
        break;
    }

    switch( src->nChannels ){
        case 4:
        cvCvtColor( tmp, dst, CV_BGRA2RGB );
        break;
        case 3:
        cvCopy( tmp, dst );
        break;
        case 1:
        cvCvtColor( tmp, dst, CV_GRAY2RGB );
        break;
    }

    QImage timg = QImage( (uchar*)dst->imageData, dst->width, dst->height, QImage::Format_RGB888 );

    if( size != NULL )                                                                                                  // Scale the image if needed using supplied transformation and aspect ratio modes
        timg = timg.scaled( *size, amode, tmode );

    QPixmap qpix = QPixmap::fromImage( timg );

    cvReleaseImage( &tmp );
    cvReleaseImage( &dst );

    RETURN qpix;
}
