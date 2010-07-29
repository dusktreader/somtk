#pragma once

#include <QtGui/QDialog>
#include "qimagehsom.h"
#include "suspect.h"
#include "qlocal.h"

namespace Ui
{
    class TrainDialog;
}

class TrainDialog : public QDialog
{
    Q_OBJECT
public:
    TrainDialog( QWidget* parent=0, QProgressDialog* pgrs=NULL );
    ~TrainDialog();
    QImageHSOM* fetchHSOM();

protected:
    void changeEvent( QEvent* e );

private:
    Ui::TrainDialog* m_ui;
    SizePlus<int> gridSz;
    SizePlus<int> featSz;
    int stepSz;
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
    void on_tableWidget_cellChanged( int row, int column );
    void on_tableWidget_cellClicked( int row, int column );
};
