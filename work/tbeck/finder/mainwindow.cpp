#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName( this, "Open Source Image", "./", "Images (*.png *.jpg *.tif)" );
    if( fileName == "" )
        return;
    rgb = cv::Mat( cv::imread( fileName.toStdString() ) );
    cv::cvtColor( rgb, ycc, CV_RGB2YCrCb );

    vector<cv::Mat> channels;
    cv::split( ycc, channels );
    gry = ~channels[0];
    ui->rawImageBox->loadImage( fileName );
}

void MainWindow::on_actionGo_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    TophatDetector detector;
    vector<double> params( 4, 0.0 );
    double detectionWidth = ui->minSlider->value();

    vector<cv::Rect> detections;

    int i=0;
    QProgressDialog pgrs( this, Qt::Popup );
    pgrs.setMaximum( int( ceil( logB( ui->maxSlider->value() / ui->minSlider->value(), ui->scaleSlider->value() ) ) ) );
    pgrs.setWindowTitle( "Detecting Stars" );
    pgrs.show();
    QApplication::processEvents();


    while( detectionWidth < ui->maxSlider->value() )
    {
        pgrs.setValue( i );
        pgrs.setLabelText( QString( "Detecting with width %1").arg(detectionWidth) );
        if( pgrs.wasCanceled() )
        {
            ui->statusBar->showMessage( "Detection Cancelled" );
            return;
        }
        QApplication::processEvents();

        double brimWidth = 3;
        double crownRadius = detectionWidth / 2 - brimWidth;
        double hatHeight = ui->sensitivitySlider->value();
        double step = ui->granularitySlider->value();
        params[0] = crownRadius;
        params[1] = brimWidth;
        params[2] = hatHeight;
        params[3] = step;
        detector.setParameters( params );

        cv::Size detectSize( (int)detectionWidth, (int)detectionWidth );
        detector.search( gry, cv::Size(), detectSize, detections, false );

        detectionWidth *= ui->scaleSlider->value();

        i++;
    }
    pgrs.hide();

    foreach( QGraphicsRectItem* r, detectBoxes )
    {
        ui->rawImageBox->scene()->removeItem( r );
        delete r;
    }
    detectBoxes.clear();

    for( unsigned int i=0; i<detections.size(); i++ )
    {
        QGraphicsRectItem* r = new QGraphicsRectItem( cvRectToQRect( detections[i]) );
        r->setPen( QPen( QBrush(Qt::red), 3 ) );
        r->setZValue( 1 );
        ui->rawImageBox->scene()->addItem( r );
        detectBoxes.push_back( r );
    }
    ui->statusBar->showMessage( QString( "Found %1 objects").arg(detections.size() ) );
}
