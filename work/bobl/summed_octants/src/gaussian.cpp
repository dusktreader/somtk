#include <unistd.h>
#include <stdio.h>

#include "cv.h"
using namespace cv;

#include "tier.h"

void kernel_print(const Mat &kernel, const int tierNumber, const double height)
{
    Size size = kernel.size();

    if (height == 0.0)
        printf("tier %d (initial)\n", tierNumber);
    else
        printf("after tier %d of height: %f\n", tierNumber, height);
    for (int i = 0; i < kernel.rows; i++) {
        printf("%d: ", i);
        for (int j = 0; j < kernel.cols; j++) {
            if (kernel.at<double>(i,j) == 0.0)
                printf("%8s ", "");
            else
                printf("%8.5f ", kernel.at<double>(i,j));
        }
        printf("\n");
    }
    printf("\n");
}

void tier_trim(const Tier *tier, Mat &m, int &tierNumber)
/*
 * removes `tier` from `m` (in-place)
 */
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
    kernel_print(m, tierNumber++, tier->z);
}

void tiers_print(Mat &m, int filterWidth)
{
    int tierNumber = 0;
    TierDecomposition *tierDecomposition;

    tierDecompositions_init(m, filterWidth);
    kernel_print(m, tierNumber++, 0.0);
    tierDecomposition = getTierDecomposition(filterWidth);
    for (int i = 0; ; i++) {
        Tier *tier = &tierDecomposition->tier[i];
        tier_trim(tier, m, tierNumber);
        if (tier->w <= 2 && tier->h <= 2)
            break;  // last entry
    }
}

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

void help(char *progName)
{
    fprintf(stderr,"syntax: %s [-w width] [-h]\n", progName);
}

int main(int argc, char *argv[])
{
    int filterWidth = 3;
    double sigma = filterWidth / 6.0;
    int opt;

    while ((opt = getopt(argc, argv, "hw:")) != -1) {
        switch (opt) {

        case 'h':
            help(argv[0]);
            exit(EXIT_FAILURE);

        case 'w':
            filterWidth = atoi(optarg);
            break;
        }
    }
    if (optind < argc) {
        help(argv[0]);
        fprintf(stderr, "extra argument(s) provided -- exiting\n");
        exit(EXIT_FAILURE);
    }
    Mat kernel2D = gaussianKernel2D(filterWidth, sigma);
    tiers_print(kernel2D, filterWidth);
}
