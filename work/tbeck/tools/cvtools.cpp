#include "cvtools.hpp"

using namespace std;

cv::Mat crop( const cv::Mat& src, const RectPlus<int>& roi )
{
    cv::Mat sub( roi.size(), src.type() );
    sub.setTo( 0 );

    if( roi.right() <= 0 || roi.bottom() <= 0 || roi.left() > src.cols || roi.top() > src.rows )
        return sub;

    RectPlus<int> srcRoi, subRoi;
    srcRoi = roi;

    srcRoi.x = max( 0, roi.x );
    srcRoi.y = max( 0, roi.y );

    subRoi.x = srcRoi.x - roi.x;
    subRoi.y = srcRoi.y - roi.y;

    subRoi.width  = srcRoi.width  = min( roi.width  - subRoi.x, src.cols - srcRoi.x );
    subRoi.height = srcRoi.height = min( roi.height - subRoi.y, src.rows - srcRoi.y );

    cv::Mat wrk( sub, subRoi );
    cv::Mat( src, srcRoi ).copyTo( wrk );

    return sub;
}

void tophat1D( const cv::Mat& src, cv::Mat& dst,
               int crownW, int brimW, int height,
               bool overX, bool overY,
               bool invert, bool norm,
               const cv::Mat& msk )
{
    ASSERT_MSG( src.cols > 0 && src.rows > 0, "The source image must have a valid size" );
    ASSERT_MSG( msk.empty() || msk.size() == src.size(), "Mask must be the same size as the source image" );
    ASSERT_MSG( height > 0 && height <=255, "The height must lie in the range [1..255]" );
    ASSERT_MSG( crownW > 1, "The crown width must be greater than 2 " );
    ASSERT_MSG( brimW > 0, "The brim width must be greater than 0" );
    ASSERT_MSG( overX != overY, "X-wise or Y-wise operations are mutually exclusive" );

    vector<cv::Mat> srcStack, dstStack;
    cv::split( src, srcStack );
    for( unsigned int idx=0; idx<srcStack.size(); idx++ )
    {
        cv::Mat wrk = srcStack[idx];
        if( invert )
            wrk = ~wrk;

        int w, h;
        h = w = 2 * brimW + crownW;

        cv::Mat dst( wrk.size(), CV_8UC1 );
        dst.setTo( 0 );

        cv::Mat crownMaskX( cv::Size(w,1), CV_8UC1 );
        for( int j=0; j<w; j++ )
            crownMaskX.at<uchar>(0,j) = j >= brimW && j < w - brimW ? 255 : 0;
        cv::Mat brimMaskX = ~crownMaskX;

        cv::Mat crownMaskY( cv::Size(1,h), CV_8UC1 );
        for( int i=0; i<h; i++ )
            crownMaskY.at<uchar>(i,0) = i >= brimW && i < h - brimW ? 255 : 0;
        cv::Mat brimMaskY = ~crownMaskY;

        cv::Mat roiX, roiY;
        cv::Scalar brimMean, crownMean;

        #pragma omp parallel for private( roiX, roiY, brimMean, crownMean )
        for( int i=h/2; i<wrk.rows-h/2; i++ )
        {
            for( int j=w/2; j<wrk.cols-w/2; j++ )
            {
                if( !msk.empty() && msk.at<uchar>(i,j) == 0 )
                    continue;
                if( overX )
                {
                    roiX = cv::Mat( wrk, cv::Rect(j-w/2,i,w,1) );
                    brimMean = cv::mean( roiX, brimMaskX );
                    crownMean = cv::mean( roiX, crownMaskX );
                }

                if( overY )
                {
                    roiY = cv::Mat( wrk, cv::Rect(j,i-h/2,1,h) );
                    brimMean = cv::mean( roiY, brimMaskY );
                    crownMean = cv::mean( roiY, crownMaskY );
                }

                int diff = (int)abs( crownMean[0] - brimMean[0] );
                if( diff > height )
                    dst.at<uchar>( i, j ) = diff;
            }
        }
        if( norm )
            cv::normalize( dst, dst, 0, 255, cv::NORM_MINMAX );
        dstStack.push_back( dst );
    }
    cv::merge( dstStack, dst );
}

void makeHisto1D( const cv::Mat& src, std::vector<double>& hist, int binCt, const cv::Mat& msk )
{
    ASSERT_MSG( src.type() == CV_8UC1, "This method can only use 8 bit single channel images" );
    ASSERT_MSG( msk.type() == CV_8UC1, "Only an 8 bit single channel mask may be used" );

    hist = vector<double>( binCt, 0.0 );
    int N = 0;

    for( int i=0; i<src.rows; i++ )
    {
        for( int j=0; j<src.rows; j++ )
        {
            if( !msk.empty() && msk.at<uchar>(i,j) == 0 )
                continue;
            hist[ src.at<uchar>( i, j ) * binCt / 256 ]++;
            N++;
        }
    }
    for( int i=0; i<binCt; i++ )
    {
        hist[i] /= N;
    }

}

void multiOtsu( const cv::Mat& src, cv::Mat& dst, int M )
{
    int L = 256;
    vector<double> p;
    makeHisto1D( src, p, L );

    cv::Mat_<double> H = cv::Mat_<double>( L, L, 0.0 );
    cv::Mat_<double> P = H.clone();
    cv::Mat_<double> S = H.clone();

    for( int u=0; u<L; u++ )
    {
        for( int v=u; v<L; v++ )
        {
            if( u == 0 )
            {
                if( v ==0 )
                {
                    P( 0, 0 ) = p[0];
                    S( 0, 0 ) = p[0];
                }
                else
                {
                    P( 0, v ) = P( 0, v-1 ) + p[v];
                    S( 0, v ) = S( 0, v-1 ) + v * p[v];
                }
            }
            else
            {
                P( u, v ) = P( 0, v ) - P( 0, u - 1 );
                S( u, v ) = S( 0, v ) - S( 0, u - 1 );
            }
            H( u, v ) = pow( S( u, v ), 2.0 ) / P( u, v );
        }
    }

    int w = L - M,
        h = M - 1;
    int divs[h];
    for( int i=0; i<h; i++ )
        divs[i] = i;
    divs[h-1] = divs[h-2];
    double varMax = 0.0;
    vector< pair<int,int> > tMax;
    while( divs[0] < w )
    {
        int i = h - 1;
        divs[i]++;
        while( divs[i] > i + w )
            divs[--i]++;
        while( ++i < h )
            divs[i] = divs[i-1] + 1;

        vector< pair<int,int> > t;
        t.push_back( pair<int,int>( 0, divs[0] ) );
        for( int i=1; i<M-1; i++ )
            t.push_back( pair<int,int>( divs[i-1]+1, divs[i] ) );;
        t.push_back( pair<int,int>( divs[h-1]+1, L-1 ) );

        double var = 0.0;
        for( unsigned int j=0; j<t.size(); j++ )
            var += H[t[j].first][t[j].second];

        if( var > varMax )
        {
            varMax = var;
            tMax = t;
        }
    }

    vector<int> clrMap(256,0);

    for( int i=0; i<M; i++ )
        for( int j=tMax[i].first; j<=tMax[i].second; j++ )
            clrMap[j] = i * 255 / ( M-1 );

    //printList( clrMap, 256, "clrMap", false );

    dst = cv::Mat( src.size(), CV_8UC1 );
    for( int i=0; i<dst.rows; i++ )
        for( int j=0; j<dst.cols; j++ )
            dst.at<uchar>( i, j ) = clrMap[ src.at<uchar>( i, j ) ];
}

void filterDoG( const cv::Mat& src, cv::Mat& dst, int size0, int size1 )
{
    cv::Mat dog1, dog2;
    cv::GaussianBlur( src, dog1, cv::Size( size0, size0 ), 0 );
    cv::GaussianBlur( src, dog2, cv::Size( size1, size1 ), 0 );
    dst = cv::abs( dog1 - dog2 );
}

cv::Mat drawPoints( const cv::Mat& src, std::vector< PointPlus<double> >& points, int radius, int width, cv::Scalar color )
{
    cv::Mat ptImg;
    if( src.channels() == 1 )
        cv::cvtColor( src, ptImg, CV_GRAY2RGB );
    else
        ptImg = src.clone();
    for( unsigned int i=0; i<points.size(); i++ )
        cv::circle( ptImg, points[i], radius, color, width );
    return ptImg;
}

cv::Mat strel( int size )
{
    return cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(size,size) );
}
