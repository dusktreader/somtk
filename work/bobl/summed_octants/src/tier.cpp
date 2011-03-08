#include <assert.h>
#include "tier.h"

static struct TierDecomposition tierDecompositions[] =
{
    /*
     * The first of the three numbers in the first member of each of
     * these sets of tuples denotes the filter width that tuple
     * applies to.
     */
    {{
            { 1, 1, 0 }  // e.g., filter width = 1
        }},
    {{
            { 2, 2, 0 },  // e.g., filter width = 2, and so on
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

#define N_ELEM_ARRAY(a) (sizeof(a)/sizeof(a[0]))

struct TierDecomposition *getTierDecomposition(int filterWidth)
/*
 * returns the decomposition array for a filter of width filterWidth.
 */
{
    // tierDecomposition[] is 0-based, but filterWidth must be positive.
    assert(0 < filterWidth 
           && filterWidth <= (int) N_ELEM_ARRAY(tierDecompositions));
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

void tierDecompositions_init(Mat &m_, int filterWidth)
/*
 * initializes the tier decompositions for a matrix "m" and a
 * filter width "filterWidth".
 */
{
    struct TierDecomposition *tierDecomposition;
    Mat m;

    m_.copyTo(m);
    tierDecomposition = getTierDecomposition(filterWidth);
    for (int i = 0; ; i++) {
        struct Tier *tier = &tierDecomposition->tier[i];
        tier_setZ(tier, m);
        if (tier->w <= 2 && tier->h <= 2)
            break;  // last entry
    }
}
