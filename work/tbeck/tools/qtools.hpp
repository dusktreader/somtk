#pragma once

#include <QImage>
#include <QPixmap>
#include <QRect>
#include <QPoint>
#include "cv.h"
#include "cxcore.h"
#include "tools.h"

/// @todo Document this file

QPixmap cvMatToQPixmap( const cv::Mat &src, QSize size=QSize(), Qt::TransformationMode tmode=Qt::FastTransformation, Qt::AspectRatioMode amode=Qt::KeepAspectRatio, unsigned int trimBorder=0 )
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

QRect cvRectToQRect( const cv::Rect& cr )
{
    return QRect( cr.x, cr.y, cr.width, cr.height );
}

QPoint cvPointToQPoint( const cv::Point& cp )
{
    return QPoint( cp.x, cp.y );
}

cv::Rect  qRectToCvRect( const QRect& qr )
{
    return cv::Rect( qr.x(), qr.y(), qr.width(), qr.height() );
}

cv::Point qPointToCvPoint( const QPoint& qp )
{
    return cv::Point( qp.x(), qp.y() );
}
