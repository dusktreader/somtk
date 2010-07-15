#ifndef QIMAGEHSOM_H                                                                                                   
#define QIMAGEHSOM_H                                                                                                   
                                                                                                                       
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
    QImageHSOM( QProgressDialog* pgrs = NULL );                                                                        
    QImageHSOM( QProgressDialog* pgrs, int w, int h, int featW, int featH, int catCt );                                
    QImageHSOM( QProgressDialog* pgrs, QString fileName );                                                             
    virtual ~QImageHSOM();                                                                                             
    virtual bool statusCheck( int iteration, std::string msg1="", std::string msg2="", int maxIters=0 );               
    bool loadSuspects( QString qDirPath, const QStringList &qFileList );                                               
};                                                                                                                     
                                                                                                                       
#endif                                                                                                                  // QIMAGEHSOM_H
