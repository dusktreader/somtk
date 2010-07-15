#ifndef TRAINDIALOG_H                                                                                                  
#define TRAINDIALOG_H                                                                                                  
                                                                                                                       
#include <QtGui/QDialog>                                                                                               
#include "qimagehsom.h"                                                                                                
#include "suspect.h"                                                                                                   
#include "qlocal.h"                                                                                                    
                                                                                                                       
namespace Ui {                                                                                                         
    class TrainDialog;                                                                                                 
}                                                                                                                      
                                                                                                                       
class TrainDialog : public QDialog {                                                                                   
    Q_OBJECT                                                                                                           
public:                                                                                                                
    TrainDialog( QWidget *parent = 0, QProgressDialog* pgrs = NULL );                                                  
    ~TrainDialog();                                                                                                    
    QImageHSOM* fetchHSOM();                                                                                           
                                                                                                                       
protected:                                                                                                             
    void changeEvent(QEvent *e);                                                                                       
                                                                                                                       
private:                                                                                                               
    Ui::TrainDialog *m_ui;                                                                                             
    int hsomW;                                                                                                         
    int hsomH;                                                                                                         
    int featureW;                                                                                                      
    int featureH;                                                                                                      
    int catCt;                                                                                                         
    int trainEpochs;                                                                                                   
    double initAlpha;                                                                                                  
    double initRad;                                                                                                    
    int annIters;                                                                                                      
    double annEpsilon;                                                                                                 
    bool addHSOM;                                                                                                      
    QMessageBox msgBox;                                                                                                
    QImageHSOM* hsom;                                                                                                  
    QProgressDialog* pgrs;                                                                                             
                                                                                                                       
private slots:                                                                                                         
    void on_buttonBox_accepted();                                                                                      
    void on_tableWidget_cellChanged(int row, int column);                                                              
    void on_tableWidget_cellClicked(int row, int column);                                                              
};                                                                                                                     
                                                                                                                       
#endif                                                                                                                  // TRAINDIALOG_H
