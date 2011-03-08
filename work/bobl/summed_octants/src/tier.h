#include "cv.h"
using namespace cv;

// in "tier.cc"
struct Tier {
    int w, h; // width and height of tier
    int d;    // inset of octagonal tier (0->square, w->diamond)
    double z; // coefficient ("z height") of tier
};

enum { MAX_TIERS = 20 }; // maximum number of tiers per decomposition
struct TierDecomposition {
    struct Tier tier[MAX_TIERS];
};

extern struct TierDecomposition *getTierDecomposition(int n);
extern void tierDecompositions_init(Mat &m, int filterWidth);
