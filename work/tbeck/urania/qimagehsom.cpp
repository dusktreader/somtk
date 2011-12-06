#include "qimagehsom.h"

using namespace std;

QImageHSOM::QImageHSOM( QProgressDialog* pgrs )
    : ImageHSOM(), pgrs(pgrs)
{}

QImageHSOM::QImageHSOM( QProgressDialog* pgrs, SizePlus<int> gridSz, SizePlus<int> featSz, int stepSz, int catCt )
    : ImageHSOM( gridSz, featSz, stepSz, catCt ), pgrs( pgrs )
{}

QImageHSOM::QImageHSOM( QProgressDialog* pgrs, QString fileName )
    : ImageHSOM( fileName.toStdString() ), pgrs( pgrs )
{}

QImageHSOM::~QImageHSOM()
{}

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
