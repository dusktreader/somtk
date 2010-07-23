#pragma once

#include "tools.hpp"

class UnitTest
{
protected:
    std::string name;
    int testCt;
public:

    UnitTest( std::string name, int testCt ) : name(name), testCt(testCt){}

    virtual ~UnitTest(){}

    virtual void run( int i ) = 0;

    bool test( std::string& report )
    {
        report += "==========================\n";
        report += "Unit Test:  " + name + "\n";
        report += "--------------------------\n";

        bool passed = true;
        for( int i=0; i<testCt; i++ )
        {
            try
            {
                run( i );
            }
            catch( LocalAssert err )
            {
                report += err.message() + "\n";
                report += "--------------------------\n";
                passed = false;
            }
        }
        if( passed )
            report += "Test Passed\n";
        report += "==========================\n";
        return passed;
    }
};


