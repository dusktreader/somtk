#include <assert.h>
#include "tier.h"

static struct TierDecomposition octagonalTierDecompositions[] =
{
    /*
     * The first of the three numbers in the first member of each of
     * these sets of tuples denotes the filter width that tuple
     * applies to.
     */
    {
    nTier: -1, // == nTier (will be set in tierDecomposition_init()
        {
            { 1, 1, 0 }  // e.g., filter width = 1
        }},
    {
    nTier: -1,
        {
            { 2, 2, 0 },  // e.g., filter width = 2, and so on
        }},
    {
    nTier: -1,
        {
            { 3, 3, 0 },
            { 3, 3, 1 },
            { 1, 1, 0 },
        }},
    {
    nTier: -1,
        {
            { 4, 4, 0 },
            { 4, 4, 1 },
            { 2, 2, 0 },
        }},
    {
    nTier: -1,
        {
            { 5, 5, 0 },
            { 5, 5, 1 },
            { 5, 5, 2 },
            { 3, 3, 0 },
            { 3, 3, 1 },
            { 1, 1, 0 },
        }},
    {
    nTier: -1,
        {
            { 6, 6, 0 },
            { 6, 6, 1 },
            { 6, 6, 2 },
            { 4, 4, 0 },
            { 4, 4, 1 },
            { 2, 2, 0 },
        }},
    {
    nTier: -1,
        {
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
    {
    nTier: -1,
        {
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
    {
    nTier: -1,
        {
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
    {
    nTier: -1,
        {
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
#define N_ELEM_ARRAY(a) (sizeof(a)/sizeof(a[0]))

static struct TierDecomposition rectangularTierDecompositions[] =
{
    /*
     * (as above, except that there are no diagonal entires (d == 0))
     */
    {
    nTier: -1,
        {
            { 1, 1, 0 }  // e.g., filter width = 1
        }},
    {
    nTier: -1,
        {
            { 2, 2, 0 },  // e.g., filter width = 2, and so on
        }},
    {
    nTier: -1,
        {
            { 3, 3, 0 },
            { 3, 1, 0 },
            { 1, 3, 0 },
            { 1, 1, 0 },
        }},
    {
    nTier: -1,
        {
            { 4, 4, 0 },
            { 4, 2, 0 },
            { 2, 4, 0 },
            { 2, 2, 0 },
            { 0, 0, 0 },
        }},
    {
    nTier: -1,
        {
            { 5, 5, 0 },
            { 5, 3, 0 },
            { 3, 5, 0 },
            { 5, 1, 0 },
            { 1, 5, 0 },
            { 3, 3, 0 },
            { 3, 1, 0 },
            { 1, 3, 0 },
            { 1, 1, 0 },
            { 0, 0, 0 },
        }},
    {
    nTier: -1,
        {
            { 6, 6, 0 },
            { 6, 4, 0 },
            { 4, 6, 0 },
            { 6, 2, 0 },
            { 2, 6, 0 },
            { 4, 4, 0 },
            { 2, 4, 0 },
            { 4, 2, 0 },
            { 2, 2, 0 },
            { 0, 0, 0 },
        }},
    {
    nTier: -1,
        {
            { 7, 7, 0 },
            { 7, 5, 0 },
            { 5, 7, 0 },
            { 7, 3, 0 },
            { 3, 7, 0 },
            { 7, 1, 0 },
            { 1, 7, 0 },
            { 5, 5, 0 },
            { 5, 3, 0 },
            { 3, 5, 0 },
            { 5, 1, 0 },
            { 1, 5, 0 },
            { 3, 3, 0 },
            { 1, 3, 0 },
            { 3, 1, 0 },
            { 1, 1, 0 },
            { 0, 0, 0 },
        }},
    {
    nTier: -1,
        {
            { 8, 8, 0 },
            { 8, 6, 0 },
            { 6, 8, 0 },
            { 8, 4, 0 },
            { 4, 8, 0 },
            { 8, 2, 0 },
            { 2, 8, 0 },
            { 6, 6, 0 },
            { 6, 4, 0 },
            { 4, 6, 0 },
            { 6, 2, 0 },
            { 2, 6, 0 },
            { 4, 4, 0 },
            { 4, 2, 0 },
            { 2, 4, 0 },
            { 2, 2, 0 },
            { 0, 0, 0 },
        }},
    {
    nTier: -1,
        {
            { 9, 9, 0 },
            { 9, 7, 0 },
            { 7, 9, 0 },
            { 9, 5, 0 },
            { 5, 9, 0 },
            { 9, 3, 0 },
            { 3, 9, 0 },
            { 9, 1, 0 },
            { 1, 9, 0 },
            { 7, 7, 0 },
            { 7, 5, 0 },
            { 5, 7, 0 },
            { 7, 3, 0 },
            { 3, 7, 0 },
            { 7, 1, 0 },
            { 1, 7, 0 },
            { 5, 5, 0 },
            { 5, 3, 0 },
            { 3, 5, 0 },
            { 5, 1, 0 },
            { 1, 5, 0 },
            { 3, 3, 0 },
            { 3, 1, 0 },
            { 1, 3, 0 },
            { 1, 1, 0 },
            { 0, 0, 0 },
        }},
    {
    nTier: -1,
        {
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
            {  0,  0, 0 },
        }},
};

/*
 *  These are set either for octagonal or rectangular decompositions
 *  in tier_init().
 */
static struct TierDecomposition *tierDecompositions = NULL;
static int nTierDecompositions = 0;


struct TierDecomposition *getTierDecomposition(int filterWidth)
/*
 * returns the decomposition array for a filter of width filterWidth.
 */
{
    // tierDecomposition[] is 0-based, but filterWidth must
    // be positive.
    assert(0 < filterWidth && filterWidth <= nTierDecompositions);
    return &tierDecompositions[filterWidth-1];
}

static void tier_setZ(struct Tier *tier, Mat &m)
/*
 * sets the z values ("heights") of a tier according to m, which is
 * modified in-place
 */
{
    int n = m.rows;
    int iUL = (n - tier->h) / 2;
    int jUL = (n - tier->w) / 2;
    int i;

    /*
     * The following is true for a Gaussian, but for other filters
     * we'll have to check.
     */
    tier->z = m.at<double>(iUL, jUL + tier->d);

    i = iUL;
    // upper trapezoid of octagon
    for (int d = tier->d; d > 0; d--) {
        for (int j = jUL + d; j < jUL + tier->w - d; j++)
            m.at<double>(i,j) -= tier->z;
        i++;
    }
    // middle rectangle of octagon
    for (; i < iUL + tier->h - tier->d; i++) {
        for (int j = jUL; j < jUL + tier->w; j++)
            m.at<double>(i,j) -= tier->z;
    }
    // lower trapezoid of octagon
    for (int d = 1; d <= tier->d; d++) {
        for (int j = jUL + d; j < jUL + tier->w - d; j++)
            m.at<double>(i,j) -= tier->z;
        i++;
    }
    assert(i == iUL + tier->h); // sanity check
}

void tier_init(Mat &m_, int filterWidth, int rectOnly)
/*
 * initializes the tier decompositions for a matrix "m" and a
 * filter width "filterWidth".
 */
{
    struct TierDecomposition *tierDecomposition;
    Mat m;

    if (rectOnly) {
        tierDecompositions = &rectangularTierDecompositions[0];
        nTierDecompositions = N_ELEM_ARRAY(rectangularTierDecompositions);
    } else {
        tierDecompositions = &octagonalTierDecompositions[0];
        nTierDecompositions = N_ELEM_ARRAY(octagonalTierDecompositions);
    }
    m_.copyTo(m);
    tierDecomposition = getTierDecomposition(filterWidth);
    tierDecomposition->nTier = 0;
    for (int i = 0; ; i++) {
        struct Tier *tier = &tierDecomposition->tier[i];
        tier_setZ(tier, m);
        tierDecomposition->nTier++;
        if (tier->w <= 2 && tier->h <= 2)
            break;  // last entry
    }
}

#ifdef TEST

#include <unistd.h>
#include <stdio.h>

#include "cv.h"
using namespace cv;

#include "gaussian.h"

void kernel_print(const Mat &kernel, const int tierNumber, const double height)
{
    Size size = kernel.size();

    if (height == 0.0)
        printf("tier %d (initial)\n", tierNumber);
    else
        printf("after tier %d of height: %8.5f\n", tierNumber, height);
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

void tier_subtract(const Tier *tier, Mat &m, int &tierNumber)
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

void tiers_print(Mat &m, const int filterWidth, const int rectOnly)
{
    int tierNumber = 0;
    TierDecomposition *tierDecomposition;

    tier_init(m, filterWidth, rectOnly);
    kernel_print(m, tierNumber++, 0.0);
    tierDecomposition = getTierDecomposition(filterWidth);
    for (int i = 0; ; i++) {
        Tier *tier = &tierDecomposition->tier[i];
        tier_subtract(tier, m, tierNumber);
        if (tier->w <= 2 && tier->h <= 2)
            break;  // last entry
    }
}

void help(const char *progName)
{
    fprintf(stderr,
            "syntax: %s [option]*\n"
            "\n"
            "  [option] is:\n"
            "  -h    help (this message)\n"
            "  -w #  use # for filter width (and height)\n"
            "  -r    use only rectangular tiers\n",
            progName);
}

int main(int argc, char *argv[])
{
    int filterWidth = 3;
    double sigma = filterWidth / 6.0;
    int opt;
    int rectOnly = 0;

    while ((opt = getopt(argc, argv, "hrw:")) != -1) {
        switch (opt) {

        case 'h':
            help(argv[0]);
            exit(EXIT_FAILURE);

        case 'r':
            rectOnly = 1;
            break;

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
    tiers_print(kernel2D, filterWidth, rectOnly);
}

#endif
