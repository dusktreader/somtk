#include "featuretest.h"

using namespace std;

FeatureTest::FeatureTest()
    : Feature(), UnitTest( "FeatureTest", 1 )
{

}

FeatureTest::~FeatureTest(){}

void FeatureTest::run( int i )
{
    switch( i )
    {
    case 0: readWriteTest(); break;
    default: break;
    }
}

void FeatureTest::readWriteTest()
{
    int testNum = 0;
    string funcName = PRINT_FUNC;

    string testFile = "/data/test.xml";
    vector<double> nData;
    for( double i=0; i<10; i++ )
        nData.push_back( i );
    setData( nData );

    cv::FileStorage fs( testFile, cv::FileStorage::WRITE );

    write( fs );
    fs.release();
    data.clear();

    fs = cv::FileStorage( testFile, cv::FileStorage::READ );
    read( fs.root() );

    ASSERT_MSG( data == nData,
                "Failed " + funcName + " test " + num2str(testNum++) + " : Read / Write test" );

}
