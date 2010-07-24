#include "local.h"

using namespace std;

vector<double> img2data( IplImage* src, int w, int h ){
    ENTER;
    ASSERT_MSG( src != NULL, "Source image may not be NULL" );
    ASSERT_MSG( w > 0, "Width must be positive" );
    ASSERT_MSG( h > 0, "Height must be positive" );
    ASSERT_MSG( src->depth == IPL_DEPTH_64F && src->nChannels == 1,
                "Image must be single channel 64 bit floating point" );

    IplImage* tmp = cvCreateImage( cvSize( w, h ), IPL_DEPTH_64F, 1 );                                                  // Create an image of the desired size
                                                                                                                        /// @todo  Figure out why we're resizing here?
    cvResize( src, tmp, CV_INTER_CUBIC );                                                                               // Resize the source image into the correct size using bi-cubic interpolation ( the best available ) Why r


    vector<double> data = vector<double>( w*h, 0.0 );
    for( int y=0; y<h; y++ )
        for( int x=0; x<w; x++ )
            data[y*w+x] = cvGetReal2D( tmp, x, y );
    cvReleaseImage( &tmp );
    RETURN data;
}

void imgInWindow( const IplImage* img, string title, int windowW ){
    ENTER;
    int key;
    int windowOpen;
    const char* ctitle = title.c_str();
    cvNamedWindow( ctitle, 0 );
    cvShowImage( ctitle, img );
    cvResizeWindow( ctitle, windowW, windowW );
    key = -1;
    windowOpen = 1;
    while( key != 1048603 && key != 1048586 && windowOpen ){
        key = cvWaitKey(0);
        windowOpen = cvGetWindowHandle(ctitle) != 0;
    }
    if( key == 1048603 )
        cvDestroyWindow( ctitle );
    RETURN;
}

int mod( int a, int b ){
    ENTER;
    ASSERT_MSG( b > 0, "Modulus divisor must be positive" );
    if( a < 0 ){
        RETURN b - ( abs(a) % b );
    }
    else{
        RETURN a % b;
    }
}

IplImage* cropImage( IplImage* img, int x, int y, int w, int h ){
    ENTER;
    ASSERT_MSG( x >= 0 && x < img->width, "x must be within bounds of image" );
    ASSERT_MSG( y >= 0 && y < img->height, "y must be within bounds of image" );
    ASSERT_MSG( x + w < img->width, "w cannot exceed bounds of image" );
    ASSERT_MSG( y + h < img->height, "h cannot exceed bounds of image" );

    IplImage* dst = cvCreateImage( cvSize(w,h), img->depth, img->nChannels);                                            // Create an empty IplImage to store the cropped image. The new image should be the same bit depth and have the same color model.
    cvSetImageROI( img, cvRect( x, y, w, h ) );                                                                         // Set the region of interest of the image to the supplied rectangle
    cvCopy( img, dst, NULL);                                                                                            // Deep copy the data from the ROI of the original image
    cvResetImageROI( img );                                                                                             // Reset the region of interest
    RETURN dst;
}

bool dummyCB( double pct ){
    ENTER;
    cout << (pct * 100) << "% complete" << endl;
    RETURN true;
}

IplImage* bilateralSmooth( IplImage* src, int spatialRadius, int colorRadius,
                           int iterations ){
    ENTER;
    IplImage* tmp = cvCloneImage( src );                                                                                /// @keyword ts1248810234
    IplImage* dst = cvCloneImage( src );
    for( int i=0; i<iterations; i++ ){
        cvSmooth( tmp, dst, CV_BILATERAL, 0, 0, spatialRadius, colorRadius );
        cvCopy( dst, tmp );
    }
    cvReleaseImage( &tmp );
    RETURN dst;
}

IplImage* blackhat( IplImage* src, int strelRad, int iterations ){
    ENTER;
    IplImage* dst = cvCloneImage( src );
    int strelW = strelRad * 2 - 1;
    IplConvKernel* strel = cvCreateStructuringElementEx( strelW, strelW,
                                                         strelW/2, strelW/2,
                                                         CV_SHAPE_ELLIPSE );
    cvMorphologyEx( src, dst, NULL, strel, CV_MOP_BLACKHAT, iterations );
    cvReleaseStructuringElement( &strel );
    RETURN dst;
}

IplImage* tophat( IplImage* src, int strelRad, int iterations ){
    ENTER;
    IplImage* dst = cvCloneImage( src );
    int strelW = strelRad * 2 - 1;
    IplConvKernel* strel = cvCreateStructuringElementEx( strelW, strelW,
                                                         strelW/2, strelW/2,
                                                         CV_SHAPE_ELLIPSE );
    cvMorphologyEx( src, dst, NULL, strel, CV_MOP_TOPHAT, iterations );
    cvReleaseStructuringElement( &strel );
    RETURN dst;
}

IplImage* dogFilter( IplImage* src, int largeRad, int smallRad ){
    ENTER;
    IplImage* smallGauss = cvCloneImage( src );
    IplImage* largeGauss = cvCloneImage( src );
    IplImage* dst = cvCloneImage( src );

    int lgw = largeRad * 2 - 1;
    int sgw = smallRad * 2 - 1;

    cvSmooth( src, smallGauss, CV_GAUSSIAN, sgw );
    cvSmooth( src, largeGauss, CV_GAUSSIAN, lgw );
    cvAbsDiff( smallGauss, largeGauss, dst );

    cvReleaseImage( &smallGauss );
    cvReleaseImage( &largeGauss );
    RETURN dst;
}

IplImage* logicalOperation( IplImage* src1, IplImage* src2, CvLogicOp op ){
    ENTER;
    IplImage* dst = cvCloneImage( src1 );
    switch( op ){
        case AND:
        cvAnd( src1, src2, dst );
        break;
        case OR:
        cvOr( src1, src2, dst );
        break;
        case NOT:
        cvNot( src1, dst );
        break;
        case XOR:
        cvXor( src1, src2, dst );
        break;
        case ADD:
        cvAdd( src1, src2, dst );
        break;
        case SUB:
        cvSub( src1, src2, dst );
        break;
        case MUL:
        cvMul( src1, src2, dst );
        break;
        case DIV:
        cvDiv( src1, src2, dst );
        break;
    }
    RETURN dst;
}

IplImage* erode( IplImage* src, int strelRad, int iterations ){
    ENTER;
    IplImage* dst = cvCloneImage( src );
    int strelW = strelRad * 2 - 1;
    IplConvKernel* strel = cvCreateStructuringElementEx( strelW, strelW,
                                                         strelW/2, strelW/2,
                                                         CV_SHAPE_ELLIPSE );
    cvErode( src, dst, strel, iterations );
    cvReleaseStructuringElement( &strel );
    RETURN dst;
}

IplImage* dilate( IplImage* src, int strelRad, int iterations ){
    ENTER;
    IplImage* dst = cvCloneImage( src );
    int strelW = strelRad * 2 - 1;
    IplConvKernel* strel = cvCreateStructuringElementEx( strelW, strelW,
                                                         strelW/2, strelW/2,
                                                         CV_SHAPE_ELLIPSE );
    cvDilate( src, dst, strel, iterations );
    cvReleaseStructuringElement( &strel );
    RETURN dst;
}

IplImage* adaptiveThreshold( IplImage* src, int strelRad,
                             double colorRadius, bool invert ){
    ENTER;
    IplImage* dst = cvCloneImage( src );
    int invertFlag = invert ? CV_THRESH_BINARY_INV : CV_THRESH_BINARY;
    cvAdaptiveThreshold( src, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C,
                         invertFlag, 2*strelRad-1, colorRadius );
    RETURN dst;
}

IplImage* meanThreshold( IplImage* src, double meanBuff, bool invert ){
    ENTER;
    ASSERT_MSG( meanBuff >= -1.0 && meanBuff <= 1.0,
                "The mean bufer must lie int the range [-1,1]" );
    IplImage* dst = cvCloneImage( src );
    CvScalar mSclr, sSclr;
    cvAvgSdv( src, &mSclr, &sSclr );
    double mean = mSclr.val[0];
    double stddev = sSclr.val[0];
    double maxVal = 0.0;
    switch( src->depth ){
        case IPL_DEPTH_8U:
        maxVal = 255.0;
        break;
        case IPL_DEPTH_32F:
        maxVal = 1.0;
        break;
        default:
        cout << "Missed depth assignment in mean threshold" << endl;
        break;
    }
    int invertFlag = invert ? CV_THRESH_BINARY_INV : CV_THRESH_BINARY;
    cvThreshold( src, dst, mean + meanBuff*stddev, maxVal, invertFlag );
    RETURN dst;
}

IplImage* otsuThreshold( IplImage* src, bool invert ){
    ENTER;
    IplImage* dst = cvCloneImage( src );
    int invertFlag = invert ? CV_THRESH_BINARY_INV : CV_THRESH_BINARY;
    invertFlag += CV_THRESH_OTSU;
    cvThreshold( src, dst, 0, 255, invertFlag );
    RETURN dst;
}

IplImage* morphOpen( IplImage* src, int strelRad, int iterations ){
    ENTER;
    IplImage* dst = cvCloneImage( src );
    int strelW = strelRad * 2 - 1;
    IplConvKernel* strel = cvCreateStructuringElementEx( strelW, strelW,
                                                         strelW/2, strelW/2,
                                                         CV_SHAPE_ELLIPSE );
    cvMorphologyEx( src, dst, NULL, strel, CV_MOP_OPEN, iterations );
    cvReleaseStructuringElement( &strel );
    RETURN dst;
}

IplImage* starMask( IplImage* src, bool showSteps ){
    ENTER;
                                                                                                                        /// @keyword ts1249760706
    IplImage* stack[50];
    int i = 0;
    stack[i] = cvtTo8u1c( src ); i++;                                                                                   // should always be 128 size?
    stack[i] = prep( stack[i-1] ); i++;
    stack[i] = simp( stack[i-1] ); i++;
                                                                                                                        //stack[i] = otsuThreshold( stack[i-1], false ); i++;
                                                                                                                        //stack[i] = dogFilter( stack[i-1], 22, 2 ); i++;
                                                                                                                        //stack[i] = logicalOperation( stack[i-2], stack[i-1], SUB ); i++;
                                                                                                                        //stack[i] = bilateralSmooth( stack[i-1], 19, 5, 3 ); i++;
                                                                                                                        //stack[i] = otsuThreshold( stack[i-1], false ); i++;
    int mx = i;

    if( showSteps ){
        int w = src->width;
        int h = src->height;
        int W = (int)pow( mx, 0.5 ) + 1;
        int H = W;
        IplImage* collage = cvCreateImage( cvSize( W * w, W * h ),
                                           IPL_DEPTH_8U, 1 );
        cvZero( collage );
        for( i=0; i<mx; i++ ){
            cvSetImageROI( collage, cvRect( w*(i%W), h*(i/H), w, h ) );
            cvCopy( stack[i], collage );
        }
        cvResetImageROI( collage );
        imgInWindow( collage, "Filter Steps", 1200 );
        cvReleaseImage( &collage );
    }

    for( i=0; i<mx-1; i++ )
        cvReleaseImage( stack+i );
    RETURN stack[mx-1];
}

IplImage* loadImage64f( string fileName, int imgW, int imgH ){
    ENTER;
    IplImage* src = cvLoadImage( fileName.c_str() );
    ASSERT_MSG( src != NULL, "Couldn't load source image file." );
    IplImage* dst = cvtTo64f1c( src, imgW, imgH );
    cvReleaseImage( &src );
    RETURN dst;
}

IplImage* cvtTo8u1c( IplImage* src, int imgW, int imgH ){
    ENTER;
    if( imgW == 0 )
        imgW = src->width;
    if( imgH == 0 )
        imgH = src->height;
    IplImage* tmp1 = cvCreateImage( IPL_SZ(src), src->depth, 1 );
    IplImage* tmp2 = cvCreateImage( IPL_SZ(src), IPL_DEPTH_8U, 1 );
    IplImage* dst  = cvCreateImage( cvSize(imgW,imgH), IPL_DEPTH_8U, 1 );
    switch( src->nChannels ){
        case 4:
        cvCvtColor( src, tmp1, CV_BGRA2GRAY );
        break;
        case 3:
        cvCvtColor( src, tmp1, CV_BGR2GRAY );
        break;
        case 1:
        cvCopy( src, tmp1 );
        break;
    }
    cvNormalize( tmp1, tmp1, 0, 255, CV_MINMAX );
    cvCvtScale( tmp1, tmp2 );
    cvResize( tmp2, dst, CV_INTER_CUBIC );                                                                              // Note that resizing has to be done after scale conversion because OpenCV doesn't know how to resize a floating point image
    cvReleaseImage( &tmp1 );
    cvReleaseImage( &tmp2 );
    RETURN dst;
}

IplImage* cvtTo64f1c( IplImage* src, int imgW, int imgH ){
    ENTER;
    if( imgW == 0 )
        imgW = src->width;
    if( imgH == 0 )
        imgH = src->height;
    IplImage* tmp1 = cvCreateImage( cvSize(imgW,imgH), src->depth, src->nChannels );
    IplImage* tmp2 = cvCreateImage( cvSize(imgW,imgH), src->depth, 1 );
    IplImage* dst = cvCreateImage( cvSize(imgW,imgH), IPL_DEPTH_64F, 1 );
    cvResize( src, tmp1, CV_INTER_CUBIC );
    switch( src->nChannels ){
        case 4:
        cvCvtColor( tmp1, tmp2, CV_BGRA2GRAY );
        break;
        case 3:
        cvCvtColor( tmp1, tmp2, CV_BGR2GRAY );
        break;
        case 1:
        cvCopy( tmp1, tmp2 );
        break;
    }
    cvCvtScale( tmp2, dst );
    cvNormalize( dst, dst, 0, 1, CV_MINMAX );
    cvReleaseImage( &tmp1 );
    cvReleaseImage( &tmp2 );
    RETURN dst;
}

IplImage* shellImage( IplImage* src, bool zero ){
    ENTER;
    IplImage* dst = cvCreateImage( IPL_SZ( src ), src->depth, src->nChannels );
    if( zero )
        cvZero( dst );
    RETURN dst;
}

IplImage* createMask( CvSize sz, CvRect mskRect, bool invert ){
    ENTER;
    IplImage* msk = cvCreateImage( sz, IPL_DEPTH_8U, 1 );
    cvZero( msk );
    cvSetImageROI( msk, mskRect );
    cvSet( msk, cvScalarAll( 255 ) );
    cvResetImageROI( msk );
    if( invert )
        cvNot( msk, msk );
    RETURN msk;
}

IplImage* createMask( IplImage* src, CvRect mskRect, bool invert ){
    ENTER;
    IplImage* msk = createMask( IPL_SZ(src), mskRect, invert );
    RETURN msk;
}

IplImage* prep( IplImage *src ){
    ENTER;
    IplImage* dst = shellImage( src );
    cvNot( src, dst );
    IplImage* msk = createMask( src,
                                cvRect( 1, 1, src->width-2, src->height-2),
                                true );
    cvSubS( dst, cvAvg( dst, msk ), dst );
    cvReleaseImage( &msk );
    cvNormalize( dst, dst, 255, 0, CV_MINMAX );
    RETURN dst;
}

IplImage* simp( IplImage* src ){
    ENTER;
    IplImage* dst = shellImage( src, true );

    int n_w = 5;
    int s_w = 3;
    CvRect n_rect = cvRect( 0, 0, n_w, n_w );
    CvRect s_rect = cvRect( 0, 0, s_w, s_w );
    IplImage* masks[100];
    int mask_ct = 0;

    for( s_rect.y=0; s_rect.y + s_rect.height < n_rect.height; s_rect.y++ )
        for( s_rect.x=0; s_rect.x + s_rect.width < n_rect.width; s_rect.x++ )
            masks[mask_ct++] = createMask( RECT_SZ( n_rect ), s_rect );

    CvScalar mean;
    CvScalar stdv;
    double minStdv, bestMean;
    int x_off = n_rect.width / 2;
    int y_off = n_rect.width / 2;
    IplImage* tmp1 = shellImage( src, true );
    vector<CvRect> rects;

    for( n_rect.y=0; n_rect.y + n_rect.height < src->height; n_rect.y++ )
        for( n_rect.x=0; n_rect.x + n_rect.width < src->width; n_rect.x++ )
            rects.push_back( n_rect );

#pragma omp parallel private( n_rect, minStdv, bestMean, stdv, mean )
    {                                                                                                                   // +omp
        char* data;
#pragma omp for nowait
        for( int i=0; i<(int)rects.size(); i++ ){
            n_rect = rects[i];
            minStdv = DBL_MAX;
            bestMean = 0;
            for( int j=0; j<mask_ct; j++ ){
#pragma omp critical
            {                                                                                                           // +omp
                cvSetImageROI( src, n_rect );
                cvAvgSdv( src, &mean, &stdv, masks[j] );
            }                                                                                                           // -omp
            if( stdv.val[0] < minStdv ){
                minStdv = stdv.val[0];
                bestMean = mean.val[0];
                }
            }
            data = tmp1->imageData +
                   ( n_rect.y + y_off ) * dst->widthStep +
                   n_rect.x + x_off;
            *data = (char)bestMean;
        }
    }                                                                                                                   // -omp
    cvResetImageROI( src );
    IplImage* tmp2 = cvtTo64f1c( src );
    cvPow( tmp2, tmp2, 2 );
    IplImage* tmp3 = cvtTo8u1c( tmp2 );
    cvThreshold( tmp3, tmp3, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU );
    cvDilate( tmp3, tmp3 );
    cvErode( tmp3, tmp3 );
    cvDilate( tmp3, tmp3 );
    double minV, maxV;
    cvMinMaxLoc( src, &minV, &maxV, NULL, NULL, tmp3 );
    cvCopy( src, dst, tmp3 );
    cvSubS( dst, cvScalar(minV), dst );
    cvNormalize( dst, dst, 255, 0, CV_MINMAX );
    cvReleaseImage( &tmp1 );
    cvReleaseImage( &tmp2 );
    cvReleaseImage( &tmp3 );
    for( int i=0; i<mask_ct; i++ )
        cvReleaseImage( masks+i );
    RETURN dst;
}

void generateRects( int r0, vector<cv::Rect> &rects0, int r1, vector<cv::Rect> &rects1, int xC, int yC, int q ){
    double dTheta, iTheta, cTheta, pTheta;
    int x, y, yP;
    int qP = q * r1 / r0;

/// @Note parallelism doesn't really help here unless the q is very high ( > 20000 )
//    omp_set_nested( true );
//    #pragma omp parallel sections private( dTheta, iTheta, cTheta, pTheta, x, y, yP )
//    { // ++omp parallel sections
//        #pragma omp section
//        { // ++omp section
            dTheta = PI / ( 2 * q );
            iTheta = dTheta / 2.0;
            x = (int)( r0 * cos( iTheta ) );
            y = (int)( r0 * sin( iTheta ) );
            rects0 = vector<cv::Rect>( 2 * q - 1 );
            rects0[0] = cv::Rect( xC - x, yC - y, 2 * x, 2 * y );
//            #pragma omp parallel for
            for( int i=1; i < q; i++ ){
                cTheta = iTheta + i * dTheta;
                pTheta = cTheta - dTheta;
                x  = (int)( r0 * cos( cTheta ) );
                y  = (int)( r0 * sin( cTheta ) );
                yP = (int)( r0 * sin( pTheta ) );
                rects0[i]   = cv::Rect( xC - x, yC - y,    2 * x, y - yP );
                rects0[i+q-1] = cv::Rect( xC - x, yC + yP,   2 * x, y - yP );
            }
//        } // --omp section

//        #pragma omp section
//        { // ++omp section
            dTheta = PI / ( 2 * qP );
            iTheta = dTheta / 2.0;
            x = (int)( r1 * cos( iTheta ) );
            y = (int)( r1 * sin( iTheta ) );
            rects1 = vector<cv::Rect>( 2 * qP - 1 );
            rects1[0] = cv::Rect( xC - x, yC - y, 2 * x, 2 * y );
//            #pragma omp parallel for
            for( int i=1; i < qP; i++ ){
                cTheta = iTheta + i * dTheta;
                pTheta = cTheta - dTheta;
                x  = (int)( r1 * cos( cTheta ) );
                y  = (int)( r1 * sin( cTheta ) );
                yP = (int)( r1 * sin( pTheta ) );
                rects1[i]    = cv::Rect( xC - x, yC - y,    2 * x, y - yP );
                rects1[i+qP-1] = cv::Rect( xC - x, yC + yP,   2 * x, y - yP );
            }
//        } // --omp section
//    } // --omp parallel
//    omp_set_nested( false );
}

void centerRects( int x, int y, vector<cv::Rect> &rects0, vector<cv::Rect> &rects1 ){
    int dx = x - ( rects0[0].x + rects0[0].width  / 2 );
    int dy = y - ( rects0[0].y + rects0[0].height / 2 );
    moveRects( dx, dy, rects0, rects1 );
}

void moveRects( int dx, int dy, vector<cv::Rect> &rects0, vector<cv::Rect> &rects1 ){
    for( int i=0; i<(int)rects0.size(); i++ ){
        rects0[i].x += dx;
        rects0[i].y += dy;
    }
    for( int i=0; i<(int)rects1.size(); i++ ){
        rects1[i].x += dx;
        rects1[i].y += dy;
    }
}

int diffRects( int &sum0, vector<cv::Rect> &rects0, int &sum1, vector<cv::Rect> &rects1, cv::Mat &igl ){
    sum0 = 0;
    sum1 = 0;
    int x0, y0, x1, y1;
    for( int i=0; i<(int)rects0.size(); i++ ){
        x0 = rects0[i].x;
        y0 = rects0[i].y;
        x1 = rects0[i].x + rects0[i].width;
        y1 = rects0[i].y + rects0[i].height;
        sum0 += igl.at<int>( y1, x1 ) + igl.at<int>( y0, x0 ) - igl.at<int>( y1, x0 ) - igl.at<int>( y0, x1 );
    }
    for( int i=0; i<(int)rects1.size(); i++ ){
        x0 = rects1[i].x;
        y0 = rects1[i].y;
        x1 = rects1[i].x + rects1[i].width;
        y1 = rects1[i].y + rects1[i].height;
        sum1 += igl.at<int>( y1, x1 ) + igl.at<int>( y0, x0 ) - igl.at<int>( y1, x0 ) - igl.at<int>( y0, x1 );
    }
    return sum1 - sum0;
}

void tester(){
    cv::Mat src( cv::imread( "/data/suspect_library/misc/test2.tif", 0 ), cv::Rect( 500, 500, 1000, 1000 ) );
    cv::Mat acc( src.size(), CV_32S );
    cv::Mat dst( src.size(), src.type() );
    dst.setTo( cv::Scalar::all( 0 ) );
    cv::Mat igl;
    cv::Mat iglSq;
    cv::integral( src, igl, iglSq, CV_32S );
    IplImage srcImg = IplImage( src );
    IplImage dstImg = IplImage( dst );
    vector<cv::Rect> rects0;
    vector<cv::Rect> rects1;
    int r0 = 50;
    int r1 = 100;
    int q  = 5;
    int x, y, sum0, sum1;
    #pragma omp parallel private( x, y, sum0, sum1, rects0, rects1 )
    {
        generateRects( r0, rects0, r1, rects1, r1, r1, q );
        for( y=r1; y < src.size().height - r1; y++ ){
            #pragma omp for
            for( x=r1; x < src.size().width - r1; x++ ){
                centerRects( x, y, rects0, rects1 );
                acc.at<int>( y, x ) = diffRects( sum0, rects0, sum1, rects1, igl );
            }
        }
    }
    imgInWindow( &srcImg, "source", 1000 );
    cv::normalize( acc, acc, 0, 255, cv::NORM_MINMAX );
    acc.convertTo( dst, dst.type() );
    //dst = src - dst;
    imgInWindow( &dstImg, "destination", 1000 );
}
