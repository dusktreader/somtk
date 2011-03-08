#include <iostream>
using namespace std;

#include "cv.h"
using namespace cv;

#include "tier.h"

void printKernel(int tier, double height, Mat kernel)
{
    Size size = kernel.size();

    if (height == 0.0)
        printf("tier %d (initial)\n", tier);
    else
        printf("after tier %d of height: %f\n", tier, height);
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

void trim(Mat m, int &tierNumber, struct Tier *tier)
{
    int n = m.rows;
    int i0 = (n - tier->h) / 2;
    int j0 = (n - tier->w) / 2;
    int i;

    i = i0;
    for (int d = tier->d; d > 0; d--) {
        for (int j = j0 + d; j < j0 + tier->w - d; j++)
            m.at<double>(i,j) -= tier->z;
        i++;
    }
    for (; i < i0 + tier->h - tier->d; i++) {
        for (int j = j0; j < j0+tier->w; j++)
            m.at<double>(i,j) -= tier->z;
    }
    for (int d = 1; d <= tier->d; d++) {
        for (int j = j0 + d; j < j0 + tier->w - d; j++)
            m.at<double>(i,j) -= tier->z;
        i++;
    }
    assert(i == i0 + tier->h);
    printKernel(tierNumber++, tier->z, m);
}

void printTiers(Mat &m, int filterWidth)
{
    int tierNumber = 0;
    struct TierDecomposition *tierDecomposition;

    tierDecompositions_init(m, filterWidth);
    printKernel(tierNumber++, 0.0, m);
    tierDecomposition = getTierDecomposition(filterWidth);
    for (int i = 0; ; i++) {
        struct Tier *tier = &tierDecomposition->tier[i];
        trim(m, tierNumber, tier);
        if (tier->w <= 2 && tier->h <= 2)
            break;  // last entry
    }
}

int main(int argc, char *argv[])
{
    int filterWidth = 10;
    double sigma = filterWidth / 6.0;
    Mat kernel1D;

    if (argc == 2)
        filterWidth = atoi(argv[1]);
    Mat kernel2D(filterWidth, filterWidth, CV_64F);

    kernel1D = getGaussianKernel(filterWidth, sigma);
    for (int i = 0; i < filterWidth; i++) {
        for (int j = 0; j < filterWidth; j++) {
            kernel2D.at<double>(i,j) =
                kernel1D.at<double>(i) * kernel1D.at<double>(j);
        }
    }
    printTiers(kernel2D, filterWidth);
}
