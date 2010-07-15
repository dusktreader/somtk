#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize( width(), height() );
    pgrs.setModal( true );
    tdlg = new TrainDialog( this, &pgrs );
    pgrs.setModal( true );

    loadClassifyDirTree( ui->classifyDirTextBox->text() );

    classifyThumbScene.addItem( &classifyThumbPixmap );
    ui->classifyThumbGraphicsView->setScene( &classifyThumbScene );
    ui->classifyThumbGraphicsView->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    classifyThumbPixmap.setPos( 0, 0 );

    connect( ui->actionOpen, SIGNAL(triggered()),
             this, SLOT(openMuses()) );
    connect( ui->addBtn, SIGNAL(clicked()),
             this, SLOT(openMuses()) );
    connect( ui->actionNew_HSOM, SIGNAL(triggered()),
             this, SLOT(newMuse()) );
    connect( ui->newBtn, SIGNAL(clicked()),
             this, SLOT(newMuse()) );
    connect( ui->actionRemove_HSOM, SIGNAL(triggered()),
             this, SLOT(killMuse()) );
    connect( ui->remBtn, SIGNAL(clicked()),
             this, SLOT(killMuse()) );

}

void MainWindow::loadClassifyDirTree( QString dirPath ){
    QDir classifyDir( dirPath );
    if( !classifyDir.exists() ){
        ui->statusBar->showMessage( "Couldn't load classify directory" );
        return;
    }

    QStringList nameFilter;
    nameFilter << "*.tif" << "*.png" << "*.jpg";                                                                        /// @todo  Add more image types
    classifyTreeModel.setFilter( QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files );
    classifyTreeModel.setNameFilters( nameFilter );
    ui->classifyDirTreeView->setModel( &classifyTreeModel );
    ui->classifyDirTreeView->setRootIndex( classifyTreeModel.index( dirPath, 0 ) );
    ui->classifyDirTreeView->setColumnHidden( 1, true );
    ui->classifyDirTreeView->setColumnHidden( 2, true );
    ui->classifyDirTreeView->setColumnHidden( 3, true );
}

MainWindow::~MainWindow()
{
    for( int i=0; i<muses.length(); i++ )
        delete muses[i];
    delete tdlg;
    delete ui;
}

void MainWindow::on_testDirBrowseBtn_clicked(){
    QString dir;
    dir = QFileDialog::getExistingDirectory( this,
                                             tr( "Open Testing Directory" ),
                                             "",
                                             QFileDialog::ShowDirsOnly );
    if( dir != "" )
        ui->testDirTextBox->setText( dir );
}

void MainWindow::on_testBtn_clicked(){
    if( muses.length() == 0 ){
        ui->statusBar->showMessage( "No classifiers loaded!" );
        return;
    }
    QDir srcDir( ui->testDirTextBox->text() );
    if( !srcDir.exists() ){
        ui->statusBar->showMessage( "Couldn't load testing directory" );
        return;
    }

    QStringList nameFilter;
    nameFilter << "*.tif" << "*.png" << "*.jpg";                                                                        /// @todo  Add more image types
    QStringList fileList = srcDir.entryList( nameFilter );
    bool alive = true;
    string title;

    QList<QTableWidgetSelectionRange> ranges;
    ranges = ui->musesTable->selectedRanges();                                                                          // Note that this should have at most 1 range
    if( ranges.length() == 0 ){
        ui->statusBar->showMessage( "No classifiers selected" );
        return;
    }
    if( ranges.length() > 1 ){
        ui->statusBar->showMessage( "Multiple classifiers selected.  Choose one." );
        return;
    }

    pgrs.show();
    int i = ranges[0].topRow();
    alive = muses[i]->loadSuspects( srcDir.absolutePath(), fileList );
    if( !alive ){
        ui->statusBar->showMessage( "Testing Cancelled" );
        return;
    }
    alive = muses[i]->classify();
    if( !alive ){
        ui->statusBar->showMessage( "Testing Cancelled" );
        return;
    }
    else
        pgrs.hide();
    string report;
    double hitRate;
    double nearRate;
    double missRate;
    muses[i]->getReport( report, hitRate, missRate, nearRate );
    ui->testDetailsTextBox->setText( QString::fromStdString( report ) );
    ui->testHitTextBox->setText(  QString::number(  hitRate * 100, 'f', 2 ) );
    ui->testMissTextBox->setText( QString::number( missRate * 100, 'f', 2 ) );
    ui->testNearTextBox->setText( QString::number( nearRate * 100, 'f', 2 ) );
    muses[i]->statusCheck( -1 );
    ui->statusBar->showMessage( "Finished Testing" );
}

void MainWindow::on_classifyDirBrowseBtn_clicked(){
    QString dir;
    dir = QFileDialog::getExistingDirectory( this,
                                             tr( "Open Classification Tree" ),
                                             "",
                                             QFileDialog::ShowDirsOnly );
    if( dir != "" ){
        ui->classifyDirTextBox->setText( dir );
        loadClassifyDirTree( dir );
    }
}

void MainWindow::on_classifyDirTextBox_editingFinished(){
    loadClassifyDirTree( ui->classifyDirTextBox->text() );
}

void MainWindow::on_classifyDirTreeView_activated( QModelIndex index ){
    ENTER;
    QString fileName = classifyTreeModel.filePath( index );
    IplImage* img = cvLoadImage( fileName.toAscii(), CV_LOAD_IMAGE_ANYCOLOR );
    IplImage* thumb = cvtTo8u1c( img );
    IplImage* msk  = starMask( thumb );
    IplImage* cmb   = cvCreateImage( cvSize( thumb->width * 2,thumb->height ),
                                     thumb->depth, thumb->nChannels );
    cvSetImageROI( cmb, cvRect( 0, 0, thumb->width, thumb->height ) );
    cvCopy( thumb, cmb );
    cvSetImageROI( cmb, cvRect( thumb->width, 0, msk->width, msk->height ) );
    cvCopy( msk, cmb );
    cvResetImageROI( cmb );
    QSize sz( ui->classifyThumbGraphicsView->width() - 5,
              ui->classifyThumbGraphicsView->height() - 5 );
    classifyThumbPixmap.setPixmap( iplToQPixmap( cmb, &sz,
                                                 Qt::SmoothTransformation,
                                                 Qt::IgnoreAspectRatio ) );
    if( muses.length() == 0 ){
        cvReleaseImage( &img );
        return;
    }

    ostringstream out;
    Suspect* suspect;
    vector<double> votes;
    for( int i=0; i<muses.length(); i++ ){
        suspect = muses[i]->createSuspect( img, msk );
        muses[i]->classify( suspect );
        votes = suspect->getCatVotes();
        printList( votes, muses[i]->getCatCt(), num2str(i), false, 3, 7, out );
        delete suspect;
    }

    ui->classifyVoteTextBox->clear();
    ui->classifyVoteTextBox->append( QString::fromStdString( out.str() ) );
    cvReleaseImage( &thumb );
    cvReleaseImage( &cmb );
    cvReleaseImage( &img );
    cvReleaseImage( &msk );
    RETURN;
}

void MainWindow::openMuses(){
    QImageHSOM* newMuse;
    QFileInfo museFile;
    QStringList files = QFileDialog::getOpenFileNames( this,
                                                       "Open Classifiers",
                                                       SOM_DIR,
                                                       "HSOM Files (*.xml)" );
    for( int i=0; i<files.length(); i++ ){
        museFile.setFile( files[i] );
        newMuse = new QImageHSOM( &pgrs, museFile.absoluteFilePath() );
        addMuse( newMuse );
    }
    ui->statusBar->showMessage( "Classifires loaded" );
}

void MainWindow::updateMuseTable(){
    while( ui->musesTable->rowCount() > 0 )
        ui->musesTable->removeRow( 0 );
    ui->musesTable->setRowCount( muses.length() );
    QString name;
    int hsomW, hsomH, featW, featH;
    QString hsomWStr, hsomHStr, featWStr, featHStr;
    for( int i=0; i<muses.length(); i++ ){
        name = QString::fromStdString( muses[i]->getName() );
        muses[i]->getSize( hsomW, hsomH );
        muses[i]->getFeatSize( featW, featH );
        hsomWStr = QString::number( hsomW );
        hsomHStr = QString::number( hsomH );
        featWStr = QString::number( featW );
        featHStr = QString::number( featH );
        ui->musesTable->setItem( i, 0, new QTableWidgetItem( name ) );
        ui->musesTable->setItem( i, 1, new QTableWidgetItem( hsomWStr ) );
        ui->musesTable->setItem( i, 2, new QTableWidgetItem( hsomHStr ) );
        ui->musesTable->setItem( i, 3, new QTableWidgetItem( featWStr ) );
        ui->musesTable->setItem( i, 4, new QTableWidgetItem( featHStr ) );
    }
}

bool MainWindow::addMuse( QImageHSOM* muse ){
    for( int i=0; i<muses.length(); i++ ){
        if( muse->getName() == muses[i]->getName() ){
            ui->statusBar->showMessage( "Classifier already added!" );
            return false;
        }
    }
    muses.push_back( muse );
    updateMuseTable();
    return true;
}

void MainWindow::killMuse(){
    QList<QTableWidgetSelectionRange> ranges;
    ranges = ui->musesTable->selectedRanges();                                                                          // Note that this should have at most 1 range
    if( ranges.length() == 0 )
        return;
    int n = ranges[0].topRow();
    int m = ranges[0].bottomRow();
    for( int i=n; i <= m; i++ )
        removeMuse( n );
}

bool MainWindow::removeMuse( int index ){
    delete muses[ index ];
    muses.removeAt( index );
    updateMuseTable();
    return true;
}

void MainWindow::newMuse(){
    tdlg->exec();
    QImageHSOM* newMuse = tdlg->fetchHSOM();
    if( newMuse == NULL )
        return;
    addMuse( newMuse );
    ui->statusBar->showMessage( "Added new classifier!" );
}

void MainWindow::on_actionRect_tester_triggered()
{
    tester();
}
