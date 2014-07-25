#pragma once

#include <QImage>
#include <QPixmap>
#include <QRect>
#include <QPoint>

#include "opencv2/core/core.hpp"

QRect   cvRectToQRect( const cv::Rect& cr );

QPoint  cvPointToQPoint( const cv::Point& cp );

cv::Rect  qRectToCvRect( QRect qr );

cv::Point qPointToCvPoint( QPoint qp );

cv::Mat imageQrgb2CVdbl( QImage qImage );

QImage imageCVdbl2Qrgb( cv::Mat_<double> cvImage );
