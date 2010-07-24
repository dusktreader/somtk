#include "colorhsom.h"

using namespace std;

const string ColorHSOM::alias = "ColorHSOM";

ColorHSOM::ColorHSOM() : HSOM(){}

ColorHSOM::ColorHSOM( const SizePlus<int>& gridSz, int catCt )
    : HSOM( gridSz, catCt )
{
    initFeatures();
}

ColorHSOM::ColorHSOM( const std::string &fileName ) : HSOM()
{
    load( fileName );
}

ColorHSOM::ColorHSOM( cv::FileNode& fn ) : HSOM()
{
    read( fn );
}

ColorHSOM::~ColorHSOM(){}

void ColorHSOM::initFeatures()
{
    for( int i=0; i<grid.l(); i++ )
        grid[i] = new ColorFeature();
}

void ColorHSOM::loadSuspects( const string &dirPath, const vector<string> &fileList )
{
    clearSuspects();
    string fileName;
    int suspectCt = fileList.size();
    int realCat;
    cv::Mat tmp, img;
    ASSERT( statusCheck( 0, "Loading Suspects", "Loading Suspects", suspectCt ) );
    for( int i=0; i<suspectCt; i++ )
    {
        ASSERT( statusCheck( i, "Loading Suspect " + num2str(i) ) );
        fileName = dirPath + OS_SEP_STR + fileList[i];
        tmp = cv::imread( fileName, 1 );
        cv::resize( tmp, img, SizePlus<int>( 128, 128 ), 0, 0, cv::INTER_LANCZOS4 );   /// @todo make this size adjustable
        realCat = char2Int( fileList[i][0] );
        suspects.push_back( new ColorSuspect( img, realCat, _catCt, grid.size(), fileList[i] ) );
    }
}

void ColorHSOM::analyzeSuspects(){}

void ColorHSOM::write( cv::FileStorage& fs )
{
    fs << alias << "{";
        HSOM::write( fs );
    fs << "}";
}

void ColorHSOM::read( const cv::FileNode& fn )
{
    cv::FileNode root = fn[alias];
    HSOM::read( root );
}
