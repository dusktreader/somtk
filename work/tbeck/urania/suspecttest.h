#include "suspect.h"
#include "unittest.hpp"

class SuspectTest : public Suspect, public UnitTest
{
public:
    SuspectTest();
    virtual ~SuspectTest();
    virtual void run( int i );

    virtual Feature* getNextFeature();

    void setANNVectorsTest();
    void setCatsTest();
};

