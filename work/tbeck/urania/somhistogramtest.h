#pragma once

#include "somhistogram.h"
#include "unittest.hpp"

#include "highgui.h"

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

