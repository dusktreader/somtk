#pragma once

#include <QSharedPointer>

#include "feature.h"

#include "tools.hpp"
#include "cvtypesplus.hpp"

#include "opencv2/core/core.hpp"

class ColorFeature : public Feature
{

private:

    static RandMaster rng;



public:

    ColorFeature();

    ColorFeature( double red, double green, double blue );
};

typedef QSharedPointer<ColorFeature> ColorFeaturePtr
