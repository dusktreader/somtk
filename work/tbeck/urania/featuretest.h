#pragma once

#include "feature.h"
#include "unittest.hpp"

class FeatureTest : public Feature, public UnitTest
{
public:
    FeatureTest();
    virtual ~FeatureTest();
    virtual void run( int i );

    void readWriteTest();
};


