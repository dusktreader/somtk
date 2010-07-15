#ifndef SOMHISTOGRAM_H                                                                                                 
#define SOMHISTOGRAM_H                                                                                                 
                                                                                                                       
#include "hexgrid.h"                                                                                                   
#include "tools.h"                                                                                                     
                                                                                                                       
class SOMHistogram : public HexGrid                                                                                    
{                                                                                                                      
private:                                                                                                               
                                                                                                                       
    /** A set of histogram bin values */                                                                               
    std::vector<double> bins;                                                                                          
                                                                                                                       
public:                                                                                                                
                                                                                                                       
    /** Constructs a SOMHistogram with default values */                                                               
    SOMHistogram();                                                                                                    
                                                                                                                       
    /** Constructs the SOMHistogram with specified size and wrapping                                                   
      * @param  w - The desired width of the histogram                                                                 
      * @param  h - The desired height of the histogram                                                                
      */                                                                                                               
    SOMHistogram( int w, int h );                                                                                      
                                                                                                                       
    virtual ~SOMHistogram();                                                                                           
                                                                                                                       
    /** Increments a slot in the histogram                                                                             
      * @param  x - The 'x' coordinate of the slot                                                                     
      * @param  y - The 'y' coordinate of the slot                                                                     
      */                                                                                                               
    void increment( int x, int y );                                                                                    
                                                                                                                       
    /** Increments a slot in the histogram                                                                             
      * @param  index - The slot index                                                                                 
      */                                                                                                               
    void increment( int index );                                                                                       
                                                                                                                       
    /** Shows a visualization of the normalized histogram */                                                           
    void showHistogram( std::string msg = "Histogram" );                                                               
                                                                                                                       
                                                                                                                       
    /** Gets the value of a bin at the given index                                                                     
      * @param  idx - The index to fetch the value from                                                                
      */                                                                                                               
    double getBin( int idx );                                                                                          
                                                                                                                       
    /** Normalizes the histogram to itself */                                                                          
    void normalize();                                                                                                  
                                                                                                                       
};                                                                                                                     
                                                                                                                       
#endif                                                                                                                  // SOMHISTOGRAM_H
                                                                                                                       
