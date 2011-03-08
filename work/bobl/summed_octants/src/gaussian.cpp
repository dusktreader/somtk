#include <iostream>
using namespace std;

#include "cv.h"
using namespace cv;

void printKernel(int tier, Mat kernel)
{
    Size size = kernel.size();

    printf("tier %d\n", tier);
    for (int i = 0; i < size.height; i++) {
        printf("%d: ", i);
        for (int j = 0; j < size.width; j++) {
            if (kernel.at<double>(i,j) == 0.0)
                printf("%8s ", "");
            else
                printf("%8.5f ", kernel.at<double>(i,j));
        }
        printf("\n");
    }
    printf("\n");
}

void trim(Mat kernel, int i0, int j0)
{
    double val = kernel.at<double>(i0,j0);

    for (int i = 0; i < kernel.rows; i++) {
        for (int j = 0; j < kernel.cols; j++) {
            if (kernel.at<double>(i,j) != 0.0)
                kernel.at<double>(i,j) -= val;
        }
    }
}

int main(int argc, char *argv[])
{
    int width = 10;
    Mat kernel1D;
    Mat kernel2D(width, width, CV_64F);
    int i, j;

    kernel1D = getGaussianKernel(width, 3);
    for (i = 0; i < width; i++) {
        for (j = 0; j < width; j++) {
            kernel2D.at<double>(i,j) =
                kernel1D.at<double>(i) * kernel1D.at<double>(j);
        }
    }
    int tier = 1;
    printKernel(tier++, kernel2D);
    trim(kernel2D, 0, 0);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 0, 1);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 0, 2);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 1, 1);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 0, 3);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 0, 4);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 1, 2);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 3, 1);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 2, 2);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 3, 2);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 4, 2);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 3, 3);
    printKernel(tier++, kernel2D);
    trim(kernel2D, 4, 3);
    printKernel(tier++, kernel2D);
}
