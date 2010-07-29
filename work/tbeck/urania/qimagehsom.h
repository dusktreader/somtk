#pragma once

#include <QImage>
#include <QPixmap>
#include <QDir>
#include <QDirModel>
#include <QFileDialog>
#include <QProgressDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMessageBox>

#include <QProgressDialog>
#include <QTimer>
#include <QStringList>
#include "imagehsom.h"
#include "suspect.h"
#include "local.h"

class QImageHSOM : public ImageHSOM
{
private:
    QProgressDialog* pgrs;
public:
    QImageHSOM( QProgressDialog* pgrs=NULL );
    QImageHSOM( QProgressDialog* pgrs, SizePlus<int> gridSz, SizePlus<int> featSz, int stepSz, int catCt );
    QImageHSOM( QProgressDialog* pgrs, QString fileName );
    virtual ~QImageHSOM();
    void loadSuspects( QString qDirPath, const QStringList &qFileList );
    virtual bool statusCheck( int iteration, std::string msg1="", std::string msg2="", int maxIters=0 );
};
