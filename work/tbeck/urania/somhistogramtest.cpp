#include "somhistogramtest.h"

using namespace std;


SOMHistogramTest::SOMHistogramTest()
    : SOMHistogram( SizePlus<int>(8,8) ), UnitTest( "SOMHistogramTest", 4 ){}

SOMHistogramTest::~SOMHistogramTest(){}

void SOMHistogramTest::run( int i )
{
    switch( i )
    {
    case 0: incrementTest(); break;
    case 1: vizHistogramTest(); break;
    case 2: binTest(); break;
    case 3: normalizeTest(); break;
    default: break;
    }
}

void SOMHistogramTest::incrementTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    reset();
    int index = 27;
    for( int i=0; i<10; i++ )
        increment( index );
    ASSERT_MSG( grid[index] == 10.0,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Increment by index test" );

    reset();
    PointPlus<int> pt( 2, 6 );
    for( int i=0; i<10; i++ )
        increment( pt );
    ASSERT_MSG( grid[pt] == 10.0,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Increment by point test" );
}

void SOMHistogramTest::vizHistogramTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    string fileName = "~/test.jpg";
    reset();
    for( int i=0; i<l(); i++ )
        for( int j=0; j<i; j++ )
            increment( i );
    cv::Mat viz = visHistogram();
    cv::imwrite( fileName, viz );

    string answer;
    while( answer != "yes" || answer != "no" )
    {
        cout << "Please enter 'yes' or 'no'" << endl;
        cout << "Is the output image (" + fileName + ") correct?" << endl;
        cin >> answer;
        transform( answer.begin(), answer.end(), answer.begin(), tolower );
    }
    ASSERT_MSG( answer == "yes",
                "Failed " + funcName + " test " + num2str(testNum++) + " : Visualize histogram test" );
}

void SOMHistogramTest::binTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    reset();
    int index = 27;
    for( int i=0; i<10; i++ )
        increment( index );
    ASSERT_MSG( bin( index ) == 10.0,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Fetch bin value by index test" );

    reset();
    PointPlus<int> pt( 2, 6 );
    for( int i=0; i<10; i++ )
        increment( pt );
    ASSERT_MSG( bin( pt ) == 10.0,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Fetch bin value by point test" );
}

void SOMHistogramTest::normalizeTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    reset();
    int index = 27;
    for( int i=0; i<10; i++ )
        increment( index );
    normalize();
    ASSERT_MSG( bin( index ) == 1.0,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Normalize test 0" );

    reset();
    int index = 27;
    for( int i=index; i<index+4; i++ )
        for( int j=1; j<i+1-index; j++ )
            increment( i );
    normalize();
    ASSERT_MSG( bin( 26 ) - 0.25 < EPSILON &&
                bin( 27 ) - 0.50 < EPSILON &&
                bin( 28 ) - 0.75 < EPSILON &&
                bin( 29 ) - 1.00 < EPSILON &&
                bin( 30 ) - 0.00 < EPSILON,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Normalize test 1" );
}
