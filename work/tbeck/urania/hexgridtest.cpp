#include "hexgridtest.h"

using namespace std;

HexGridTest::HexGridTest():
        HexGrid<int>( SizePlus<int>(8,8) ), UnitTest( "HexGridTest", 4 )
{

}

HexGridTest::~HexGridTest(){}

void HexGridTest::run( int i )
{
    switch( i )
    {
    case 0: coordsTest(); break;
    case 1: indexTest(); break;
    case 2: neighborIndicesTest(); break;
    case 3: neighborhoodTest(); break;
    default: break;
    }
}

void HexGridTest::coordsTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    ASSERT_MSG( PointPlus<int>( 3, 3 ) == coords(27),
                "Failed " + funcName + " test " + num2str(testNum++) + " : Coorinate test 0" );
    ASSERT_MSG( PointPlus<int>( 5, 6 ) == coords(53),
                "Failed " + funcName + " test " + num2str(testNum++) + " : Coorinate test 1" );

}

void HexGridTest::indexTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    ASSERT_MSG( index( PointPlus<int>( 3, 3 ) ) == 27,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Index test 0" );
    ASSERT_MSG( index( PointPlus<int>( 5, 6 ) ) == 53,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Index test 1" );
}

void HexGridTest::neighborIndicesTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    vector<int> indices;

    indices = neighborIndices( 27 );
    ASSERT_MSG( indices[0] == 26 &&
                indices[1] == 28 &&
                indices[2] == 19 &&
                indices[3] == 20 &&
                indices[4] == 35 &&
                indices[5] == 36,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Non-wrapping center neighbor test 0" );

    indices = neighborIndices( 35 );
    ASSERT_MSG( indices[0] == 34 &&
                indices[1] == 36 &&
                indices[2] == 26 &&
                indices[3] == 27 &&
                indices[4] == 42 &&
                indices[5] == 43,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Non-wrapping center neighbor test 1" );

    indices = neighborIndices( 24 );
    ASSERT_MSG( indices[0] == 31 &&
                indices[1] == 25 &&
                indices[2] == 16 &&
                indices[3] == 17 &&
                indices[4] == 32 &&
                indices[5] == 33,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Wrapping side neighbor test 0" );

    indices = neighborIndices( 32 );
    ASSERT_MSG( indices[0] == 39 &&
                indices[1] == 33 &&
                indices[2] == 31 &&
                indices[3] == 24 &&
                indices[4] == 47 &&
                indices[5] == 40,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Wrapping side neighbor test 1" );

    indices = neighborIndices( 0 );
    ASSERT_MSG( indices[0] ==  7 &&
                indices[1] ==  1 &&
                indices[2] == 63 &&
                indices[3] == 56 &&
                indices[4] == 15 &&
                indices[5] ==  8,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Wrapping corner neighbor test 0" );

    indices = neighborIndices( 56 );
    ASSERT_MSG( indices[0] == 63 &&
                indices[1] == 57 &&
                indices[2] == 48 &&
                indices[3] == 49 &&
                indices[4] ==  0 &&
                indices[5] ==  1,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Wrapping corner neighbor test 1" );
}


void HexGridTest::neighborhoodTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    vector<int> indices = neighborhood( 3, 27 );
    set<int> nSet( indices.begin(), indices.end() );
    set<int> cSet;

    for( int i= 2; i<= 5; i++ ) cSet.insert( i );
    for( int i= 9; i<=13; i++ ) cSet.insert( i );
    for( int i=17; i<=22; i++ ) cSet.insert( i );
    for( int i=24; i<=30; i++ ) cSet.insert( i );
    for( int i=33; i<=38; i++ ) cSet.insert( i );
    for( int i=41; i<=45; i++ ) cSet.insert( i );
    for( int i=50; i<=53; i++ ) cSet.insert( i );

    ASSERT_MSG( nSet.size() == indices.size(),
                "Failed " + funcName + " test " + num2str(testNum++) + " : Non-duplicate neighbor test" );

    ASSERT_MSG( nSet == cSet,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Correct neighbors test" );
}


