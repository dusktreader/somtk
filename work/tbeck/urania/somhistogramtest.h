#pragma once

#include "somhistogram.h"
#include "unittest.hpp"

#include "highgui.h"
#include <algorithm>
#include "cvtools.hpp"

class SOMHistogramTest : public SOMHistogram, public UnitTest
{
public:
    SOMHistogramTest();
    virtual ~SOMHistogramTest();
    virtual void run( int i );

    void incrementTest();
    void vizHistogramTest();
    void binTest();
    void normalizeTest();
};

