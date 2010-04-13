#pragma once

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "ml.h"
#include "cvaux.h"

#define SHOWW( img )   { cv::imshow( #img, img ); cv::waitKey(0); }
#define SHOWN( img ) { cv::imshow( #img, img ); }
