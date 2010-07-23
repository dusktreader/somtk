#include "suspecttest.h"

using namespace std;

SuspectTest::SuspectTest()
    : Suspect( "SuspectTest", 3, 5, SizePlus<int>(8,8) ), UnitTest( "SuspectTest", 2 ){}

SuspectTest::~SuspectTest(){}

void SuspectTest::run( int i )
{
    switch( i )
    {
    case 0: setANNVectorsTest(); break;
    case 1: setCatsTest(); break;
    default: break;
    }
}

Feature* SuspectTest::getNextFeature(){ return NULL; }

void SuspectTest::setANNVectorsTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    int index = 27;
    cv::Mat_<double> input( SizePlus<int>(hist.l(),1), 0.0 );
    cv::Mat_<double> output( SizePlus<int>(catCt(),1), 0.0 );

    incrementHistogram( index );
    setANNVectors( input, output );
    ASSERT_MSG( input(0,index) - 1.0 < EPSILON,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Set ANN vectos input test 1" );
    ASSERT_MSG( output(0,realCat()) - 1.0 < EPSILON,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Set ANN vectos output test" );

    hist.reset();
    for( int i=index; i<index+5; i++ )
        for( int j=0; j<i-index+1; j++ )
            incrementHistogram( i );
    ASSERT_MSG( input(0,index+0) - 1 < EPSILON &&
                input(0,index+1) - 2 < EPSILON &&
                input(0,index+2) - 3 < EPSILON &&
                input(0,index+3) - 4 < EPSILON &&
                input(0,index+1) - 0 < EPSILON,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Set ANN vectos input test 2" );
}

void SuspectTest::setCatsTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;


    cv::Mat_<double> output( SizePlus<int>(catCt(),1), 0.0 );
    for( int i=0; i<catCt(); i++ )
        output(0,i) = i * 0.1;
    setCats( output );

    ASSERT_MSG( ( output(0,0) - _cats(0,0) ) + ( output(0,0) - 0.0 ) < EPSILON &&
                ( output(0,1) - _cats(0,1) ) + ( output(0,1) - 0.1 ) < EPSILON &&
                ( output(0,2) - _cats(0,2) ) + ( output(0,2) - 0.2 ) < EPSILON &&
                ( output(0,3) - _cats(0,3) ) + ( output(0,3) - 0.3 ) < EPSILON &&
                ( output(0,4) - _cats(0,4) ) + ( output(0,4) - 0.4 ) < EPSILON,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Set cats test" );
}
