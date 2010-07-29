#include "traindialog.h"
#include "ui_traindialog.h"

using namespace std;

TrainDialog::TrainDialog( QWidget *parent, QProgressDialog* pgrs ) : QDialog(parent), m_ui(new Ui::TrainDialog), pgrs(pgrs)
{
    m_ui->setupUi(this);
    int i = 2;
    m_ui->tableWidget->item( 0, 0 )->setText( "." );
    m_ui->tableWidget->item( 1, 0 )->setText( "./newHSOM.xml" );
    hsomW = m_ui->tableWidget->item( i++, 0 )->text().toInt();
    hsomH = m_ui->tableWidget->item( i++, 0 )->text().toInt();
    featureW = m_ui->tableWidget->item( i++, 0 )->text().toInt();
    featureH = m_ui->tableWidget->item( i++, 0 )->text().toInt();
    catCt = m_ui->tableWidget->item( i++, 0 )->text().toInt();
    trainEpochs = m_ui->tableWidget->item( i++, 0 )->text().toInt();
    initAlpha = m_ui->tableWidget->item( i++, 0 )->text().toDouble();
    initRad = m_ui->tableWidget->item( i++, 0 )->text().toDouble();
    annIters = m_ui->tableWidget->item( i++, 0 )->text().toInt();
    annEpsilon = m_ui->tableWidget->item( i++, 0 )->text().toDouble();
    addHSOM = m_ui->tableWidget->item( i++, 0 )->checkState() == Qt::Checked;
    hsom = NULL;
}

TrainDialog::~TrainDialog()
{
    delete m_ui;
}

void TrainDialog::changeEvent( QEvent* e )
{
    QDialog::changeEvent( e );
    switch ( e->type() )
    {
    case QEvent::LanguageChange:
        m_ui->retranslateUi( this );
        break;
    default:
        break;
    }
}

void TrainDialog::on_tableWidget_cellClicked( int row, int col )
{
    if( col == 0 )
        return;
    QString tmp;
    switch( row )
    {
        case 0:                                                                                                         //The training directory dialog propmpt
            tmp = QFileDialog::getExistingDirectory( this,
                                                     "Select Training Directory",
                                                     "",
                                                     QFileDialog::ShowDirsOnly );
            if( tmp != "" )
                m_ui->tableWidget->item( row, col-1 )->setText( tmp );
            break;
        case 1:
            tmp = QFileDialog::getSaveFileName( this,
                                                "Select HSOM Save File",
                                                "",
                                                "HSOM Files ( *.xml )" );
            if( tmp != "" )
                m_ui->tableWidget->item( row, col-1 )->setText( tmp );
            break;
    }
}

void TrainDialog::on_tableWidget_cellChanged( int row, int col )
{
    if( col != 0 )
        return;
    QTableWidgetItem* item = m_ui->tableWidget->item( row, col );
    int tint;
    double tdouble;
    bool ok;
    switch( row )
    {
        case  2:                                                                                                        //hsomW
            tint = item->text().toInt( &ok );
            if( !ok || tint < 6 || tint > 64 )
                item->setText( QString::number( hsomW ) );
            else
                gridSz.w = tint;
            break;
        case  3:                                                                                                        //hsomH
            tint = item->text().toInt( &ok );
            if( !ok || tint < 6 || tint > 64)
                item->setText( QString::number( hsomH ) );
            else
                gridSz.h = tint;
            break;
        case  4:                                                                                                        //featureW
            tint = item->text().toInt( &ok );
            if( !ok || (tint&1) == 0 || tint < 3 || tint > 49 )
                item->setText( QString::number( featureW ) );
            else
                featSz.w = tint;
            break;
        case  5:                                                                                                        //featureH
            tint = item->text().toInt( &ok );
            if( !ok || (tint&1) == 0 || tint < 3 || tint > 49 )
                item->setText( QString::number( featureH ) );
            else
                featSz.h = tint;
            break;
        case  6:                                                                                                        //catCt
            tint = item->text().toInt( &ok );
            if( !ok || tint < 2 )
                item->setText( QString::number( catCt ) );
            else
                catCt = tint;
            break;
        case  7:                                                                                                        //trainEpochs
            tint = item->text().toInt( &ok );
            if( !ok || tint < 10 || tint > 1000 )
                item->setText( QString::number( trainEpochs ) );
            else
                trainEpochs = tint;
            break;
        case  8:                                                                                                        //initAlpha
            tdouble = item->text().toDouble( &ok );
            if( !ok || tdouble <= 0.0 || tdouble > 1.0 )
                item->setText( QString::number( initAlpha, 'f', 2 ) );
            else
                initAlpha = tdouble;
            break;
        case  9:                                                                                                        //initRad
            tdouble = item->text().toDouble( &ok );
            if( !ok || tdouble <= 0.0 || tdouble > 0.5 )
                item->setText( QString::number( initRad, 'f', 2 ) );
            else
                initRad = tdouble;
            break;
        case 10:                                                                                                        //annIters
            tint = item->text().toInt( &ok );
            if( !ok || tint < 0 || tint > 1000 )
                item->setText( QString::number( annIters ) );
            else
                annIters = tint;
            break;
        case 11:                                                                                                        //annEpsilon
            tdouble = item->text().toDouble( &ok );
            if( !ok || tdouble < 0.0 || tdouble > 0.5 )
                item->setText( QString::number( annEpsilon, 'f', 4 ) );
            else
                annEpsilon= tdouble;
            break;
        case 12:                                                                                                        //addHSOM
            addHSOM = item->checkState() == Qt::Checked; break;
        }
}

void TrainDialog::on_buttonBox_accepted()
{
    QDir trainDir( m_ui->tableWidget->item( 0, 0 )->text() );
    if( !trainDir.exists() )
    {
        msgBox.setText( "Invalid trainig directory!" );
        msgBox.exec();
        return;
    }
    QStringList nameFilter;
    nameFilter << "*.tif" << "*.png" << "*.jpg";                                                                        /// @todo  Add more image types
    QStringList trainFiles = trainDir.entryList( nameFilter, QDir::Files | QDir::NoDotAndDotDot );
    if( trainFiles.length() < 1 )
    {
        msgBox.setText( "No training files in specified directory!" );
        msgBox.exec();
        return;
    }
    QFileInfo hsomFile( m_ui->tableWidget->item( 1, 0 )->text() );
    if( !hsomFile.absoluteDir().isReadable() )
    {
        msgBox.setText( "Invalid file name or lack of permissions" );
        msgBox.exec();
        return;
    }

    if( hsom != NULL )
        delete hsom;
    hsom = new QImageHSOM( pgrs, gridSz, featSz, stepSz, catCt );

    pgrs->show();
    try
    {
        hsom->loadSuspects( trainDir.absolutePath(), trainFiles );
        hsom->train( trainEpochs, initAlpha, initRad, annIters, annEpsilon );
        hsom->save( hsomFile.absoluteFilePath().toStdString() );
        ASSERT( m_ui->tableWidget->item( 12, 0 )->checkState() == Qt::Checked );
    }
    catch( LocalAssert err )
    {
        delete hsom;
        hsom = NULL;
    }
    pgrs->hide();
}

QImageHSOM* TrainDialog::fetchHSOM()
{
    return hsom;
}
