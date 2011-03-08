#include <iostream>
using namespace std;

#include "cv.h"
using namespace cv;

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

void trim(Mat kernel, int &tier, int w, int h, int d0)
{
    int n = kernel.rows;
    int i0 = (n - h) / 2;
    int j0 = (n - w) / 2;
    int i;
    double val = kernel.at<double>(i0,j0+d0);

    i = i0;
    for (int d = d0; d > 0; d--) {
        for (int j = j0 + d; j < j0 + w - d; j++)
            kernel.at<double>(i,j) -= val;
        i++;
    }
    for (; i < i0 + h - d0; i++) {
        for (int j = j0; j < j0+w; j++)
            kernel.at<double>(i,j) -= val;
    }
    for (int d = 1; d <= d0; d++) {
        for (int j = j0 + d; j < j0 + w - d; j++)
            kernel.at<double>(i,j) -= val;
        i++;
    }
    assert(i == i0+h);
    printKernel(tier++, val, kernel);
}

int main(int argc, char *argv[])
{
    int filterWidth = 10;
    double sigma = filterWidth / 6.0;
    Mat kernel1D;
    int i, j;
    struct Tier {
        int w, h, d;
    };
    struct {
        struct Tier tier[20];
    } tierDecomposition[] = {
        {{
            { 0, 0, 0 }, // should not be used
            }},
        {{
            { 1, 1, 0 }
            }},
        {{
            { 2, 2, 0 },
            }},
        {{
            { 3, 3, 0 },
            { 3, 3, 1 },
            { 1, 1, 0 },
            }},
        {{
            { 4, 4, 0 },
            { 4, 4, 1 },
            { 2, 2, 0 },
            }},
        {{
            { 5, 5, 0 },
            { 5, 5, 1 },
            { 5, 5, 2 },
            { 3, 3, 0 },
            { 3, 3, 1 },
            { 1, 1, 0 },
            }},
        {{
            { 6, 6, 0 },
            { 6, 6, 1 },
            { 6, 6, 2 },
            { 4, 4, 0 },
            { 4, 4, 1 },
            { 2, 2, 0 },
            }},
        {{
            { 7, 7, 0 },
            { 7, 7, 1 },
            { 7, 7, 2 },
            { 5, 5, 0 },
            { 7, 7, 3 },
            { 5, 5, 1 },
            { 5, 5, 2 },
            { 3, 3, 0 },
            { 3, 3, 1 },
            { 1, 1, 0 },
            }},
        {{
            { 8, 8, 0 },
            { 8, 8, 1 },
            { 8, 8, 2 },
            { 6, 6, 0 },
            { 8, 8, 3 },
            { 6, 6, 1 },
            { 6, 6, 2 },
            { 4, 4, 0 },
            { 4, 4, 1 },
            { 2, 2, 0 },
            }},
        {{
            { 9, 9, 0 },
            { 9, 9, 1 },
            { 9, 9, 2 },
            { 7, 7, 0 },
            { 9, 9, 3 },
            { 5, 7, 0 },
            { 7, 5, 0 },
            { 9, 9, 4 },
            { 7, 7, 2 },
            { 5, 5, 0 },
            { 7, 7, 3 },
            { 5, 5, 1 },
            { 5, 5, 2 },
            { 3, 3, 0 },
            { 3, 3, 1 },
            { 1, 1, 0 },
            }},
        {{
            { 10, 10, 0 },
            { 10, 10, 1 },
            { 10, 10, 2 },
            {  8,  8, 0 },
            { 10, 10, 3 },
            {  8,  6, 0 },
            {  6,  8, 0 },
            { 10, 10, 4 },
            {  8,  8, 2 },
            {  6,  6, 0 },
            {  8,  8, 3 },
            {  6,  6, 1 },
            {  6,  6, 2 },
            {  4,  4, 0 },
            {  4,  4, 1 },
            {  2,  2, 0 },
            }},
    };

    if (argc == 2)
        filterWidth = atoi(argv[1]);
    assert(filterWidth <= 10); // for now
    Mat kernel2D(filterWidth, filterWidth, CV_64F);

    kernel1D = getGaussianKernel(filterWidth, sigma);
    for (i = 0; i < filterWidth; i++) {
        for (j = 0; j < filterWidth; j++) {
            kernel2D.at<double>(i,j) =
                kernel1D.at<double>(i) * kernel1D.at<double>(j);
        }
    }
    int tierNumber = 0;

    printKernel(tierNumber++, 0.0, kernel2D);
    for (int i = 0; ; i++) {
        struct Tier *tier = &tierDecomposition[filterWidth].tier[i];
        trim(kernel2D, tierNumber, tier->w, tier->h, tier->d);
        if (tier->w <= 2 && tier->h <= 2)
            break;  // last entry
    }
}
