#include <unistd.h>
#include <stdio.h>

#include <cv.h>
using namespace cv;

#include "tier.h"
#include "gaussian.h"

int filterWidth = 10;
// try this with nothing but rectangular filters (d == 0)
Tier tiers[] = {
//    w  h  d
    { 10, 10, 0 },
    {  8, 10, 0 },
    { 10,  8, 0 },
    {  6, 10, 0 },
    { 10,  6, 0 },
    {  4, 10, 0 },
    { 10,  4, 0 },
    {  2, 10, 0 },
    { 10,  2, 0 },
    {  8,  8, 0 },
    {  8,  6, 0 },
    {  6,  8, 0 },
    {  8,  4, 0 },
    {  4,  8, 0 },
    {  8,  2, 0 },
    {  2,  8, 0 },
    {  6,  6, 0 },
    {  6,  4, 0 },
    {  4,  6, 0 },
    {  6,  2, 0 },
    {  2,  6, 0 },
    {  4,  4, 0 },
    {  4,  2, 0 },
    {  2,  4, 0 },
    {  2,  2, 0 },
    {  0,  0, 0 }, // end of tiers
};

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
    int iUL = (n - tier->h) / 2;
    int jUL = (n - tier->w) / 2;
    int i;
    double z = m.at<double>(iUL,jUL+tier->d);

    i = iUL;
    for (int d = tier->d; d > 0; d--) {
        for (int j = jUL + d; j < jUL + tier->w - d; j++)
            m.at<double>(i,j) -= z;
        i++;
    }
    for (; i < iUL + tier->h - tier->d; i++) {
        for (int j = jUL; j < jUL+tier->w; j++)
            m.at<double>(i,j) -= z;
    }
    for (int d = 1; d <= tier->d; d++) {
        for (int j = jUL + d; j < jUL + tier->w - d; j++)
            m.at<double>(i,j) -= z;
        i++;
    }
    assert(i == iUL + tier->h);
    kernel_print(m, tierNumber++, z);
}

void tiers_print(Mat &m, int filterWidth)
{
    int tierNumber = 0;

    kernel_print(m, tierNumber++, 0.0);
    for (int i = 0; ; i++) {
        Tier *tier = &tiers[i];
        if (tier->w <= 0 && tier->h <= 0)
            break;  // last entry
        tier_trim(tier, m, tierNumber);
    }
}

void help(char *progName)
{
    fprintf(stderr,"syntax: %s [-w width] [-h]\n", progName);
}

int main(int argc, char *argv[])
{
    double sigma = filterWidth / 6.0;

    Mat kernel2D = gaussianKernel2D(filterWidth, sigma);
    tiers_print(kernel2D, filterWidth);
}
