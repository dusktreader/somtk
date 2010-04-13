#include "imageview.h"
#include "ui_imageview.h"

using namespace std;

ImageView::ImageView(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::ImageView)
{
    ui->setupUi(this);
    _zoomFactor = 0.1;
    _zoomScale  = 1.0;
    _zoomMax    = 16.0;
    _zoomMin    = 1.0;

    setScene( new QGraphicsScene(this) );
    displayItem = NULL;

    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    setDragMode( QGraphicsView::ScrollHandDrag );

    imgName = "None";
}

ImageView::~ImageView()
{
    delete ui;
    delete this->scene();
}

void ImageView::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


bool ImageView::loadImage( const QString& fileName )
{
    QPixmap displayPix( fileName );
    loadImage( displayPix, fileName );
    return true;
}

bool ImageView::loadImage( const QPixmap& displayPix, const QString& imgName )
{
    scene()->setSceneRect( displayPix.rect() );
    if( displayItem != NULL )
        scene()->removeItem( displayItem );
    displayItem = scene()->addPixmap( displayPix );
    displayItem->setZValue( 0.0 );
    this->imgName = imgName;
    return true;
}

QString ImageView::getName() const
{
    return imgName;
}

void ImageView::resizeEvent( QResizeEvent* )
{
    redrawView();
}

void ImageView::redrawView()
{
    QPointF anchor = mapToScene( rect().center() );
    fitInView( scene()->sceneRect(), Qt::KeepAspectRatioByExpanding );  /// @todo make the aspect ratio type a property
    scale( _zoomScale, _zoomScale );
    centerOn( anchor );
}

void ImageView::wheelEvent( QWheelEvent* event )
{
    zoom( event->delta() );
    event->accept();
}

void ImageView::zoomIn()
{
    zoom( 1 );
}

void ImageView::zoomOut()
{
    zoom( -1 );
}

void ImageView::zoom( int direction )
{
    double newScale;
    if( direction > 0 )
    {
        newScale = _zoomScale * ( 1.0 + _zoomFactor );
        if( newScale <= _zoomMax )
        {
                _zoomScale = newScale;
                redrawView();
        }
    }
    else
    {
        newScale = _zoomScale * ( 1.0 - _zoomFactor );
        if( newScale >= _zoomMin )
        {
            _zoomScale = newScale;
            redrawView();
        }
    }
}

double ImageView::zoomFactor() const
{
    return _zoomFactor;
}

void   ImageView::setZoomFactor( double val )
{
    _zoomFactor = val;
    _zoomFactor = max( val, ZOOM_EPS );
    _zoomFactor = min( _zoomFactor, 1.0 );
}


double ImageView::zoomScale() const
{
    return _zoomScale;
}

void   ImageView::setZoomScale( double val )
{
    _zoomScale = max( val, _zoomMin );
    _zoomScale = min( _zoomScale, _zoomMax );
}

double ImageView::zoomMax() const{
    return _zoomMax;
}

void   ImageView::setZoomMax( double val )
{
    _zoomMax = min( val, ZOOM_MAX );
    if( _zoomMax < _zoomMin )
        _zoomMin = _zoomMax;
    if( _zoomScale > _zoomMax )
        _zoomScale = _zoomMax;
    redrawView();
}

double ImageView::zoomMin() const
{
    return _zoomMin;
}

void   ImageView::setZoomMin( double val )
{
    _zoomMin = max( val, ZOOM_MIN );
    if( _zoomMin > _zoomMax )
        _zoomMax = _zoomMin;
    if( _zoomScale < _zoomMin )
        _zoomScale = _zoomMin;
    redrawView();
}
