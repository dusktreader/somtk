#include "hsom.h"
#include "colorhsom.h"

using namespace std;

int main()
{
    string imgDir = "/home/d3x874/astrocv/trunk/lib/demo_lib/combinded";
    vector<string> fileList;
    for( int i=1; i<=20; i++ )
    {
        fileList.push_back( string( "0_" ) + ( i < 10 ? "0" : "" ) + num2str( i ) + ".jpg" );
        fileList.push_back( string( "1_" ) + ( i < 10 ? "0" : "" ) + num2str( i ) + ".jpg" );
    }

    HSOM* hsom = new ColorHSOM( SizePlus<int>(20,20), 2 );
    hsom->loadSuspects( imgDir, fileList );
    try
    {
        hsom->train( 10, 0.1, 0.5, 1000, 1e-10 );
    }
    catch( LocalAssert err )
    {
        cout << err.message() << endl;
    }
    catch(...)
    {
        cout << "unknown exception" << endl;
    }
    cv::Mat viz = hsom->visualize();
    SHOWW( viz );
    hsom->save( "testhsom.xml" );
    hsom->load( "testhsom.xml" );
}
