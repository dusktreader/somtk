#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QWheelEvent>

#include <iostream>

#define ZOOM_EPS .001
#define ZOOM_MAX 100.0
#define ZOOM_MIN ZOOM_EPS

namespace Ui {
    class ImageView;
}

class ImageView : public QGraphicsView {
    Q_OBJECT
    Q_PROPERTY( double zoomFactor READ zoomFactor WRITE setZoomFactor )
    Q_PROPERTY( double zoomScale  READ zoomScale  WRITE setZoomScale  )
    Q_PROPERTY( double zoomMax    READ zoomMax    WRITE setZoomMax    )
    Q_PROPERTY( double zoomMin    READ zoomMin    WRITE setZoomMin    )
    //Q_PROPERTY( Qt::AspectRatioMode aspectRatioMode READ aspectRatio)

protected:

    double  _zoomFactor;
    double  _zoomScale;
    double  _zoomMax;
    double  _zoomMin;
    void    zoomIn();
    void    zoomOut();
    void    zoom( int direction );

    void redrawView();

    QGraphicsPixmapItem* displayItem;
    QString imgName;

public:
    ImageView(QWidget *parent = 0);
    ~ImageView();

    double zoomFactor() const;
    void   setZoomFactor( double val );

    double zoomScale() const;
    void   setZoomScale( double val );

    double zoomMax() const;
    void   setZoomMax( double val );

    double zoomMin() const;
    void   setZoomMin( double val );

    bool   loadImage( const QString& fileName );
    bool   loadImage( const QPixmap& displayPix, const QString& imgName );

    QString getName() const;

    virtual void wheelEvent( QWheelEvent* event );
    virtual void resizeEvent( QResizeEvent* );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ImageView *ui;
};

