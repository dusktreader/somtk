#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->boxColorLbl->setAutoFillBackground( true );
    setDetectionColor( Qt::green );
    this->connect( ui->boxWidthSlider, SIGNAL(valueChanged(double)), SLOT(redrawDetections()));
    this->connect( ui->actionClear, SIGNAL(triggered()), SLOT(clearDetections()));
    this->connect( ui->ringRadio, SIGNAL(toggled(bool)), SLOT(redrawDetections()) );
    connect( ui->adjustmentsDock, SIGNAL(visibilityChanged(bool)), ui->actionShow_Adjustments, SLOT(setDisabled(bool)));
    connect( ui->actionShow_Adjustments, SIGNAL(triggered()), ui->adjustmentsDock, SLOT(show()));
    connect( ui->adjustmentsDock, SIGNAL(visibilityChanged(bool)), ui->actionHide_Adjustments, SLOT(setEnabled(bool)));
    connect( ui->actionHide_Adjustments, SIGNAL(triggered()), ui->adjustmentsDock, SLOT(close()));
    ui->adjustmentsDock->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearDetections()
{
    foreach( QGraphicsItem* r, detectionItems )
    {
        ui->rawImageBox->scene()->removeItem( r );
        delete r;
    }
    detectionItems.clear();
}

void MainWindow::redrawDetections()
{
    clearDetections();
    for( unsigned int i=0; i<detections.size(); i++ )
    {
        QGraphicsItem* r;


        if( ui->ringRadio->isChecked() )
        {
            r = new QGraphicsEllipseItem( cvRectToQRect(detections[i]) );
            ((QGraphicsEllipseItem*)r)->setPen( QPen( QBrush(detectionColor), ui->boxWidthSlider->intValue() ) );
        }
        else
        {
            r = new QGraphicsRectItem( cvRectToQRect( detections[i]) );
            ((QGraphicsRectItem*)r)->setPen( QPen( QBrush(detectionColor), ui->boxWidthSlider->intValue() ) );
        }

        r->setZValue( 1 );
        ui->rawImageBox->scene()->addItem( r );
        detectionItems.push_back( r );
    }
}

void MainWindow::setDetectionColor( const QColor& color )
{
    detectionColor = color;
    QPalette pal( ui->boxColorLbl->palette() );
    pal.setColor( QPalette::Window, detectionColor );
    ui->lineColorFrame->setPalette( pal );
    ui->boxColorLbl->setPalette( pal );
    redrawDetections();
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
    //QString fileName = QFileDialog::getOpenFileName( this, "Open Source Image", "./", "Images (*.png *.jpg *.tif)" );
    
    QString fileName = "./samller.png";
    if( fileName == "" )
        return;
    rgb = cv::Mat( cv::imread( fileName.toStdString() ) );
    cv::cvtColor( rgb, ycc, CV_RGB2YCrCb );

    vector<cv::Mat> channels;
    cv::split( ycc, channels );
    gry = ~channels[0];
    ui->rawImageBox->loadImage( fileName );
    clearDetections();
}

void MainWindow::on_actionGo_triggered()
{
    TophatDetector detector;
    vector<double> params( 4, 0.0 );
    double detectionWidth = ui->minSlider->value();

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

    redrawDetections();
    ui->statusBar->showMessage( QString( "Found %1 objects").arg(detections.size() ) );
}

void MainWindow::on_boxColorBtn_clicked()
{
    QColor color = QColorDialog::getColor( detectionColor, this, "Select Detection Box Color" );
    if( !color.isValid() )
        return;
    setDetectionColor( color );
}
