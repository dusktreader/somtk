#include "somhistogram.h"                                                                                              
                                                                                                                       
using namespace std;                                                                                                   
                                                                                                                       
SOMHistogram::SOMHistogram() : HexGrid(){                                                                              
    ENTER;                                                                                                             
    RETURN;                                                                                                            
}                                                                                                                      
                                                                                                                       
SOMHistogram::SOMHistogram( int w, int h ) : HexGrid( w, h ){                                                          
    ENTER;                                                                                                             
    bins = vector<double>( l, 0.0 );                                                                                   
    RETURN;                                                                                                            
}                                                                                                                      
                                                                                                                       
SOMHistogram::~SOMHistogram(){                                                                                         
    ENTER;                                                                                                             
    RETURN;                                                                                                            
}                                                                                                                      
                                                                                                                       
void SOMHistogram::increment( int x, int  y ){                                                                         
    ENTER;                                                                                                             
    increment( getIndex( x, y ) );                                                                                     
    RETURN;                                                                                                            
}                                                                                                                      
                                                                                                                       
void SOMHistogram::increment( int index ){                                                                             
    ENTER;                                                                                                             
    bins[ index ]++;                                                                                                   
    RETURN;                                                                                                            
}                                                                                                                      
                                                                                                                       
void SOMHistogram::normalize(){                                                                                        
    ENTER;                                                                                                             
    double minVal = bins[0];                                                                                           
    double maxVal = bins[0];                                                                                           
    for( int i=1; i<l; i++ ){                                                                                          
        minVal = min( bins[i], minVal );                                                                               
        maxVal = max( bins[i], maxVal );                                                                               
    }                                                                                                                  
    for( int i=0; i<l; i++ )                                                                                           
        bins[i] = ( bins[i] - minVal ) / maxVal;                                                                       
    RETURN;                                                                                                            
}                                                                                                                      
                                                                                                                       
double SOMHistogram::getBin( int idx ){                                                                                
    ENTER;                                                                                                             
    RETURN bins[idx];                                                                                                  
}                                                                                                                      
                                                                                                                       
void SOMHistogram::showHistogram( string msg ){                                                                        
    ENTER;                                                                                                             
    double maximum = 0.0;                                                                                              
    for( int i=0; i<l; i++ )                                                                                           
        maximum = max( bins[i], maximum );                                                                             
    int radius = 10;                                                                                                   
    double buff = 2.5 * radius;;                                                                                       
    int imgW = (int)( 0.5 * buff + buff * w );                                                                         
    int imgH = (int)( buff * h * HG_B );                                                                               
    IplImage* map = cvCreateImage( cvSize( imgW, imgH ), IPL_DEPTH_64F, 1 );                                           
    cvSet( map, cvScalarAll( 0.0 ) );                                                                                  
    int x, y;                                                                                                          
    double xR, yR;                                                                                                     
    for( int i=0; i<l; i++ ){                                                                                          
        getCoords( i, x, y );                                                                                          
        getRealCoords( x, y, xR, yR );                                                                                 
        cvCircle( map, cvPoint( (int)(xR * buff + buff/2.0),                                                           
                                (int)(yR * buff + buff/2.0) ),                                                         
                  radius, cvScalarAll( bins[i] / maximum ), CV_FILLED );                                               
    }                                                                                                                  
    imgInWindow( map, msg );                                                                                           
    cvReleaseImage( &map );                                                                                            
    RETURN;                                                                                                            
}                                                                                                                      
