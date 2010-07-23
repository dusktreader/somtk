#include "imagehsom.h"

using namespace std;

const string ImageHSOM::alias = "ImageHSOM";

ImageHSOM::ImageHSOM() : HSOM(){}

ImageHSOM::ImageHSOM( const SizePlus<int>& sz, const SizePlus<int>& featSz, int catCt )
    : HSOM( sz, catCt ), _featSz(featSz)
{
    initFeatures();
}

ImageHSOM::ImageHSOM( string fileName ) : HSOM()
{
    load( fileName );
}

ImageHSOM::ImageHSOM( cv::FileNode& fn ) : HSOM()
{
    read( fn );
}

ImageHSOM::~ImageHSOM(){}

void ImageHSOM::featSz()
{
    return _featSz;
}

void ImageHSOM::initFeatures()
{
    for( int i=0; i<l; i++ )
        grid[i] = new HuFeature();
}

void ImageHSOM::loadSuspects( string dirPath, const vector<string> &fileList )
{
    clearSuspects();
    string fileName;
    int suspectCt = fileList.size();
    int realCat;
    cv::Mat tmp, img, msk;
    ASSERT( statusCheck( 0, "Loading Suspects", "Loading Suspects", suspectCt ) );
    for( int i=0; i<suspectCt; i++ )
    {
        ASSERT( statusCheck( i, "Loading Suspect " + num2str(i) ) );
        fileName = dirPath + OS_SEP_STR + fileList[i];
        tmp = cv::imread( fileName, 0 );
        cv::resize( tmp, img, SizePlus<int>( 128, 128 ), 0, 0, cv::INTER_LANCZOS4 );
        msk = starMask( img );
        realCat = char2Int( fileList[i][0] );
        suspects.push_back( new ImageSuspect( img, mask, realCat, _catCt, grid.size(), featSz(), fileList[i] ) );
    }
}

bool ImageHSOM::analyzeSuspects()
{
    int max_feats = 100000;
    int featCt = 0;
    vector<double> hus;
    HuFeature* feat;
    vector<double> huMean(  7, 0.0 );
    vector<double> huStdv(  7, 0.0 );
    vector<double> huAlpha( 7, 0.0 );
    double eps = 0.0625;                                                                                                /// @todo  these should probably be parameterized....
    int sigmaStep = 2;
    double t;

    ASSERT( statusCheck( 0, "Analyzing suspect Hu values", "Analyzing suspect Hu values", suspects.size() ) );
    ASSERT_MSG( suspects.size() > 0, "No suspects to analyze!" );

    feat = dynamic_cast<HuFeature*>( suspects[0]->getNextFeature() );
    ASSERT_MSG( feat != NULL, "No features with which to train!" );
    hus = feat->getOriginalHus();
    for( int i=0; i<7; i++ )
    {
        huMean[i] = hus[i];
        huStdv[i] = 0;
    }
    featCt++;

    for( vector<Suspect*>::iterator s = suspects.begin(); s != suspects.end(); s++ )
    {
        Suspect* suspect = *s;
        ASSERT( statusCheck( i++, "Analyzing Suspect " + suspect->getName() ) );

        if( featCt < max_feats )
        {
            feat = dynamic_cast<HuFeature*>( suspect->getNextFeature() );
            while( feat != NULL && featCt < max_feats )
            {
                hus = feat->getOriginalHus();
                for( int i=0; i<7; i++ )
                {
                    t = hus[i] - huMean[i];
                    huMean[i] += t / featCt;
                    huStdv[i] += t * ( hus[i] - huMean[i] );
                }
                featCt++;
                feat = dynamic_cast<HuFeature*>( suspect->getNextFeature() );
            }
        }
    }

    for( int i=0; i<7;ij++ )
    {
        huStdv[i] = sqrt( huStdv[i] / ( featCt - 1 ) );
        huAlpha[i] = log( 1 / eps - 1 ) / ( sigmaStep * huStdv[i] );
    }

    HuFeature::setSigmoidParams( huMean, huStdv, huAlpha );
}

void ImageHSOM::write( cv::FileStorage& fs )
{
    fs << alias << "{";
        fs << "featW" << _featSz.w;
        fs << "featH" << _featSz.h;
        fs << "sigmoid_parameters" << "{";
            vector<double> huMean(  7, 0.0 );
            vector<double> huStdv(  7, 0.0 );
            vector<double> huAlpha( 7, 0.0 );
            fs << "hu_mean_values"         << "[" << huMean  << "]";
            fs << "hu_standard_deviations" << "[" << huStdv  << "]";
            fs << "hu_alpha_values"        << "[" << huAlpha << "]";
        fs << "}";
        HSOM::write( fs );
    fs << "}";
}

void ImageHSOM::read( const cv::FileNode& fn )
{
    cv::FileNode root = fn[alias];
    _featSz = SizePlus<int>( (int)root["featW"], (int)root["featH"] );

    cv::FileNode huNode = root["sigmoid_parameters"];
    vector<double> huMean;
    vector<double> huStdv;
    vector<double> huAlpha;
    huNode["hu_mean_values"]         >> huMean;
    huNode["hu_standard_deviations"] >> huStdv;
    huNode["hu_alpha_values"]        >> huAlpha;
    HuFeature::setSigmoidParams( huMean, huStdv, huAlpha );

    HSOM::read( root );
}
