#pragma once

#include <set>

#include "hexgrid.hpp"
#include "unittest.hpp"

class HexGridTest : public HexGrid<int>, public UnitTest
{
public:
    HexGridTest();
    virtual ~HexGridTest();
    virtual void run( int i );

    void coordsTest();
    void indexTest();
    void neighborIndicesTest();
    void neighborhoodTest();
};

