#include "qimagehsom.h"

using namespace std;

QImageHSOM::QImageHSOM( QProgressDialog* pgrs ) : ImageHSOM(){
    ENTER;
    this->pgrs = pgrs;
    RETURN;
}

QImageHSOM::QImageHSOM( QProgressDialog* pgrs,
                        int w, int h, int featW, int featH,
                        int catCt ) :
ImageHSOM( w, h, featW, featH, catCt ){
    ENTER;
    this->pgrs = pgrs;
    RETURN;
}

QImageHSOM::QImageHSOM( QProgressDialog* pgrs, QString fileName ) :
ImageHSOM( fileName.toStdString() ){
    ENTER;
    this->pgrs = pgrs;
    RETURN;
}

QImageHSOM::~QImageHSOM(){
    ENTER;
    RETURN;
}

bool QImageHSOM::loadSuspects( QString qDirPath, const QStringList &qFileList ){
    ENTER;
    string dirPath = qDirPath.toStdString();
    vector<string> fileList;
    for( int i=0; i<qFileList.length(); i++ )
        fileList.push_back( qFileList.at(i).toStdString() );
    bool success = ImageHSOM::loadSuspects( dirPath, fileList );
    RETURN success;
}

bool QImageHSOM::statusCheck( int iteration, string msg1,
                              string msg2, int maxIters ){
    ENTER;
    if( pgrs == NULL ){
        RETURN true;
    }
    if( pgrs->wasCanceled() == true ){
        pgrs->reset();
        RETURN false;
    }
    if( maxIters != 0 ){
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

    RETURN true;
}
