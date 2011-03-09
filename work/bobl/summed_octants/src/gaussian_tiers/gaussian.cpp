#include <cv.h>
using namespace cv;

#include "gaussian.h"

Mat gaussianKernel2D(const int filterWidth, const double sigma)
{
    Mat kernel1D;
    Mat kernel2D(filterWidth, filterWidth, CV_64F);

    kernel1D = getGaussianKernel(filterWidth, sigma);
    for (int i = 0; i < filterWidth; i++) {
        for (int j = 0; j < filterWidth; j++) {
            kernel2D.at<double>(i,j) =
                kernel1D.at<double>(i) * kernel1D.at<double>(j);
        }
    }
    return kernel2D;
}
