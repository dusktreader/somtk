#include "cvq.h"

using namespace std;

/*
QPixmap cvMatToQPixmap( const cv::Mat& src, QSize size,
                        Qt::TransformationMode tmode, Qt::AspectRatioMode amode,
                        unsigned int trimBorder )
{
    QImage timg;
    cv::Mat tmp( src.size(), CV_8UC3 );
    if( src.type() == CV_8UC3 )
        cv::cvtColor( src, tmp, CV_BGR2RGB );
    else
        cv::cvtColor( src, tmp, CV_GRAY2RGB );
    timg = QImage( tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888 );
    if( !size.isEmpty() )
    {
        if( trimBorder > 0 )
        {
            size.rwidth() -= 2 * trimBorder;
            size.rheight() -= 2  * trimBorder;
        }
        timg = timg.scaled( size, amode, tmode );
    }
    return QPixmap::fromImage( timg, Qt::ColorOnly );
}
*/

QRect cvRectToQRect( const cv::Rect& cr )
{
    return QRect( cr.x, cr.y, cr.width, cr.height );
}

QPoint cvPointToQPoint( const cv::Point& cp )
{
    return QPoint( cp.x, cp.y );
}

cv::Rect  qRectToCvRect( QRect qr )
{
    return cv::Rect( qr.x(), qr.y(), qr.width(), qr.height() );
}

cv::Point qPointToCvPoint( QPoint qp )
{
    return cv::Point( qp.x(), qp.y() );
}

cv::Mat imageQrgb2CVdbl( QImage qImage )
{
    cv::Mat_<double> cvImage = cv::Mat_<double>( qImage.height(), qImage.width(), 0.0 );
    for( int i = 0; i < qImage.height(); i++ )
    {
        for( int j = 0; j < qImage.width(); j++ )
        {
            cvImage( i, j ) = qGray( qImage.pixel( j, i ) );
        }
    }

    cv::normalize( cvImage, cvImage, 0.0, 1.1, cv::NORM_MINMAX);
    return cvImage;
}

QImage imageCVdbl2Qrgb( cv::Mat_<double> cvImage )
{
    QImage qImage( cvImage.size().width, cvImage.size().height, QImage::Format_RGB888 );
    for( int i = 0; i < qImage.height(); i++ )
    {
        for( int j = 0; j < qImage.width(); j++ )
        {
            double val = cvImage( i, j );
            uchar r = val * 32.0 / 11.0;
            uchar g = val * 32.0 / 16.0;
            uchar b = val * 32.0 / 5.0;
            qImage.setPixel( j, i, qRgb( r, g, b ) );
        }
    }
    return qImage;
}
