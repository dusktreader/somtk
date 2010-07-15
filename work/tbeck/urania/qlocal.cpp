#include "qlocal.h"                                                                                                    
                                                                                                                       
using namespace std;                                                                                                   
                                                                                                                       
QPixmap iplToQPixmap( IplImage* src,                                                                                   
                      QSize* size,                                                                                     
                      Qt::TransformationMode tmode,                                                                    
                      Qt::AspectRatioMode amode ){                                                                     
    ENTER;                                                                                                             
                                                                                                                       
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
