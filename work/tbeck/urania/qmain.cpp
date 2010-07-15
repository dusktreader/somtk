#include <QtGui/QApplication>                                                                                          
#include "mainwindow.h"                                                                                                
#include "local.h"                                                                                                     
                                                                                                                       
int main(int argc, char *argv[])                                                                                       
{                                                                                                                      
    ENTER;                                                                                                             
    QApplication a(argc, argv);                                                                                        
    MainWindow w;                                                                                                      
    w.show();                                                                                                          
    int success = a.exec();                                                                                            
    RETURN success;                                                                                                    
}                                                                                                                      
