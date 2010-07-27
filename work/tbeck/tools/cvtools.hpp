#pragma once

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "ml.h"

#include "tools.hpp"
#include "cvtypesplus.hpp"

#define CV_WHIT cv::Scalar(  255, 255, 255 )
#define CV_NRED cv::Scalar(    0,   0, 255 )
#define CV_LRED cv::Scalar(   50,  50, 255 )
#define CV_DRED cv::Scalar(    0,   0, 205 )
#define CV_NGRN cv::Scalar(    0, 255,   0 )
#define CV_LGRN cv::Scalar(   50, 255,  50 )
#define CV_NBLU cv::Scalar(  255,   0,   0 )
#define CV_DGRN cv::Scalar(    0, 205,   0 )
#define CV_LBLU cv::Scalar(  255,  50,  50 )
#define CV_DBLU cv::Scalar(  205,   0,   0 )
#define CV_GY50 cv::Scalar(  127, 127, 127 )
#define CV_BLAK cv::Scalar(    0,   0,   0 )

#define SHOWW( img )   { cv::namedWindow( #img, 0 ), cv::imshow( #img, img ); cv::waitKey(0); }
#define SHOWN( img ) {  cv::namedWindow( #img, 0 ), cv::imshow( #img, img ); }

#ifndef EPSILON
#define EPSILON 1e-10
#endif

/** Prints a cv::Mat to an output stream
  * @param  M          - The matrix to print
  * @param  label      - The string label to apply to the matrix print out
  * @param  abbreviate - Only print the corners of the matrix
  * @param  prec       - The precision to print for matrices composed of double and float values
  * @param  width      - The width of printed matrix values
  * @param  out        - The output stream in which to print the matrix
  */
template<class T>
void printCVMat( cv::Mat_<T> M, std::string label="matrix", bool abbreviate=false, int prec=2, int width=0, std::ostream &out=std::cout )
{
    out << label << std::endl
        << "------------" << std::endl;
    for( int i=0; i<M.rows; i++ )
    {
        if( abbreviate && i == 3 && M.rows > 10 )
        {
            out << "..." << std::endl;
            i = M.rows - 4;
        }
        out << "| " << std::flush;
        for( int j=0; j<M.cols; j++ )
        {
            if( abbreviate && j == 3 && M.cols > 10 )
            {
                out << "..." << std::flush;
                j = M.cols - 4;
            }
            out << M(i,j) << " " << std::flush;
        }
        out << "|" << std::endl;
    }
}

/** Prints the file, line, matrix name, and matrix values
  * @param  mat - The matrix to report
  */
#define DB_REP_CVMAT( mat )      { std::ostringstream op; \
                                   op << __FILE__ ":" << __LINE__ << ":" << endl; \
                                   printCVMat( mat, #mat, true, 2, 0, op ); \
                                   std::cout << op.str() << std::flush; }

/** Crops the specified rectangle out of the image.
  * @param  src - The image from which to crop the selection
  * @param  roi - The rectangular region to crop from the image
  * @note   Parts of the rectangle that fall out of the source image are set to black
  */
cv::Mat crop( const cv::Mat& src, const RectPlus<int>& roi );

/** Performs a 1D TopHat filter on the image
  * @param  src    - The source image to filter
  * @param  dst    - The filtered version of the source image
  * @param  crownW - The width of the crown of the filter
  * @param  brimW  - The width of the brim of the filter
  * @param  height - The height of the crown above the brim
  * @param  overX  - Filter the image in an x-wise fasion
  * @param  overY  - Filter the image in an y-wise fasion
  * @param  invert - Invert the source image before filtering
  * @param  norm   - Normalize the result image after filtering
  * @param  msk    - The image filtering binary mask
  */
void tophat1D( const cv::Mat& src, cv::Mat& dst, int crownW=10, int brimW=5, int height=3, bool overX=true, bool overY=false, bool invert=false, bool norm=false, const cv::Mat& msk=cv::Mat() );

/** Makes a 1D histogram of the matrix values
  * @param  src   - The matrix to profile
  * @param  hist  - The vector of doubles in which to store the histogram values
  * @param  binCt - The number of bins in which to store the profile
  * @param  msk   - The binary operation mask for the source image
  */
void makeHisto1D( const cv::Mat& src, std::vector<double>& hist, int binCt, const cv::Mat& msk=cv::Mat() );

/** Performs a multi-level Otsu thresholding operation
  * @param  src - The source image on which to perform the thresholding
  * @param  dst - The image that will store the results of the thresholding
  * @param  M   - The number of levels to use for thresholding
  * @note   Any value of M greater than 5 will be extremely slow
  */
void multiOtsu( const cv::Mat& src, cv::Mat& dst, int M=4 );

/** Perform a Difference of Gaussians filterin operation
  * @param  src   - The source image on which to perform the DoG filter
  * @param  dst   - The image that will store the results of the DoG filter
  * @param  size0 - The width of the first Gaussian aperature
  * @param  size1 - The width of the second Gaussian aperature
  */
void filterDoG( const cv::Mat& src, cv::Mat& dst, int size0, int size1 );

/** Visualizes points as colored circles on an image
  * @param  src    - The source image on which to draw the points
  * @param  points - The vector of points to draw
  * @param  radius - The radius of the circles which shall be drawn
  * @param  width  - The thickness of the circles which shall be drawn
  * @param  color  - The color of the circles which shall be drawn
  * @return A copy of the source image with colored circles drawn on it
  */
cv::Mat drawPoints( const cv::Mat& src, std::vector< PointPlus<double> >& points, int radius=3, int width=1, cv::Scalar color=CV_NRED );

/** Creates a structuring element for OpenCV morphological image filters
  * @param  size - The width of the structuring element
  * @return A structuring element of the specified size
  */
cv::Mat strel( int size );
