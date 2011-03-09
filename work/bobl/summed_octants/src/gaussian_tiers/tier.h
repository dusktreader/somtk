#include "cv.h" // for Mat
using namespace cv;

// in "tier.cc"
struct Tier {
    int w, h; // width and height of tier
    int d;    // inset of octagonal tier (0->square, w->diamond)
    double z; // coefficient ("z height") of tier
};

enum { MAX_TIERS = 32 }; // maximum number of tiers per decomposition
struct TierDecomposition {
    // these are statically initialized
    struct Tier tier[MAX_TIERS]; // initialized
    // this is computed (and defaults to zero)
    int nTier;  // number of tiers in this decomposition
};

extern struct TierDecomposition *getTierDecomposition(const int n);
extern void tier_init(const Mat &m, const int filterWidth, const int rectOnly);
