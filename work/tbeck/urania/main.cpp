#include "imagehsom.h"                                                                                                 
#include "local.h"                                                                                                     
                                                                                                                       
using namespace std;                                                                                                   
                                                                                                                       
int main(int argc, char **argv ){                                                                                      
    srand( time(NULL) );                                                                                               
                                                                                                                       
    ArgStruct args;                                                                                                    
                                                                                                                       
    parseArgs( argc, argv, &args );                                                                                    
                                                                                                                       
    if( args.showUsage == true ){                                                                                      
        cout << usage( argv[0] );                                                                                      
        exit( 1 );                                                                                                     
    }                                                                                                                  
                                                                                                                       
    if( args.test == true ){                                                                                           
        cout << "test" << endl;                                                                                        
        exit( 1 );                                                                                                     
    }                                                                                                                  
                                                                                                                       
    if( args.trainDir != "<none>" && args.inSOM != "<none>" ){                                                         
        cout << "Cannot train and load from file.  Choose one" << endl;                                                
        cout << usage( argv[0] );                                                                                      
        exit( -1 );                                                                                                    
    }                                                                                                                  
                                                                                                                       
    if( args.trainDir == "<none>" && args.inSOM == "<none>" ){                                                         
        cout << "Must either train or load an SOM.  Choose one" << endl;                                               
        cout << usage( argv[0] );                                                                                      
        exit( -2 );                                                                                                    
    }                                                                                                                  
                                                                                                                       
    Urania* muse;                                                                                                      
    if( args.trainDir != "<none>" ){                                                                                   
        muse = new Urania( args.w, args.h, args.featW, args.featH,                                                     
                           args.wrap, args.filt,                                                                       
                           args.catCt, args.gaussRad, args.normalize );                                                
        vector<string> fileList;                                                                                       
        if( listDir( args.trainDir, fileList ) < 0 ){                                                                  
            delete muse;                                                                                               
            cout << "Can't get a list of files from " << args.trainDir << endl;                                        
            exit( -3 );                                                                                                
        }                                                                                                              
        muse->train( args.trainDir, fileList,                                                                          
                     args.somEpochs, args.initAlpha, args.initR,                                                       
                     args.annIters, args.annEps );                                                                     
    }                                                                                                                  
    else{                                                                                                              
        muse = new Urania();                                                                                           
        muse->loadFile( args.inSOM );                                                                                  
    }                                                                                                                  
                                                                                                                       
    if( args.outSOM != "<none>" )                                                                                      
        muse->saveFile( args.outSOM );                                                                                 
                                                                                                                       
    if( args.targetImg != "<none>" ){                                                                                  
        IplImage* img = loadImage32f( args.targetImg );                                                                
        if( img == NULL ){                                                                                             
            delete muse;                                                                                               
            cvReleaseImage( &img );                                                                                    
            cout << "couldn't load target image. " << endl;                                                            
            exit( -3 );                                                                                                
        }                                                                                                              
        int predClass = muse->classify( img );                                                                         
        cout << args.targetImg << " classified as: " <<                                                                
                int2str( predClass ) << endl;                                                                          
        cvReleaseImage( &img );                                                                                        
    }                                                                                                                  
                                                                                                                       
    if( args.targetDir != "<none>" ){                                                                                  
        vector<string> targetImgs;                                                                                     
        listDir( args.targetDir, targetImgs );                                                                         
        string testReport;                                                                                             
        double hitRate;                                                                                                
        double missRate;                                                                                               
        double nearRate;                                                                                               
        muse->test( args.targetDir, targetImgs,                                                                        
                    testReport, hitRate, missRate, nearRate );                                                         
        cout << "============" << endl <<                                                                              
                "Test Results" << endl <<                                                                              
                "------------" << endl <<                                                                              
                testReport <<                                                                                          
                "------------" << endl <<                                                                              
                "Hits:        " << dbl2str( 100 * hitRate, 2, 5 ) <<                                                   
                "Near Misses: " << dbl2str( 100 * hitRate, 2, 5 ) <<                                                   
                "Misses:      " << dbl2str( 100 * hitRate, 2, 5 ) << endl;                                             
    }                                                                                                                  
                                                                                                                       
    delete muse;                                                                                                       
    return 0;                                                                                                          
}                                                                                                                      
