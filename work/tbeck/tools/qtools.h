#pragma once

#include <QImage>
#include <QPixmap>
#include <QRect>
#include <QPoint>
#include "cv.h"
#include "cxcore.h"
#include "tools.h"

QPixmap cvMatToQPixmap( const cv::Mat &src, QSize size=QSize(), Qt::TransformationMode tmode=Qt::FastTransformation, Qt::AspectRatioMode amode=Qt::KeepAspectRatio, unsigned int trimBorder=0 );

QRect   cvRectToQRect( const cv::Rect& cr );

QPoint  cvPointToQPoint( const cv::Point& cp );

cv::Rect  qRectToCvRect( const QRect& qr );

cv::Point qPointToCvPoint( const QPoint& qp );
