#pragma once

#include "cv.h"
#include "cxcore.h"
#include "cxerror.h"
#include "highgui.h"
#include "ml.h"

#include "omp.h"

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <string>

#include "tools.h"

/** Fetches the size of an IplImage as a CvSize object */
#define IPL_SZ( img ) cvSize( img->width, img->height )

#define RECT_SZ( rct ) cvSize( rct.width, rct.height )

/** A constant describing the conversion factor between FWHM and sigma */
#define FWHM_FACTOR 2.3548200450309493

#define UNCATEGORIZED -1

#define MAX_CATS 100

/** Converts an IplImage into a double vector of data
  * @param  img  - The image to convert
  * @param  w    - The desired width of the data vector
  * @param  h    - The desired height of the data vector
  * @return A float vector reperesenting the image data
  * @note   The source image must be single channel normalized 64 bit float
  * @note   The resulting array is normalized bicubic resized
  */
std::vector<double> img2data( IplImage* img, int w, int h );

/** Shows an IplImage in a titled window
  * @param  img   - The image to show
  * @param  title - The title of the window
  */
void imgInWindow( const IplImage* img, std::string title="Image", int windowW=500 );

/** Provides a mathematically correct integer modulus function
  * @param  a - The dividend
  * @param  b - The divisor
  * @return The result of the modulus operation
  */
int mod( int a, int b );

/** Creates an empty shell that has the same size, depth, and channel count
  * @param  src  - The source Image
  * @param  zero - Specifies whether the result image should be zeroed.
  * @return An image that is an empty shell of the source image
  */
IplImage* shellImage( IplImage* src, bool zero=false );

/** Creates an image mask of a certain size
  * @param  imgSize - The size of the mask to create
  * @param  mskRect - The rectangle for the masked area
  * @param  invert  - Should the mask be inverted?
  * @return A mask image
  * @todo   Parameter checking
  */
IplImage* createMask( CvSize imgSize, CvRect mskRect, bool invert=false );

/** Creates an image mask the same size as a given image
  * @param  src     - The image to create the mask for
  * @param  mskRect - The rectangle for the masked area
  * @param  invert  - Should the mask be inverted?
  * @return A mask image
  * @todo   Parameter checking
  */
IplImage* createMask( IplImage* src, CvRect mskRect, bool invert=false );

/** Crops a section from an image
  * @param  img - The image to crop
  * @param  x   - The left side of the cropping rectangle
  * @param  y   - The top side of the cropping rectangle
  * @param  w   - The width of the cropping rectangle
  * @param  h   - The height of the cropping rectangle
  * @return A cropped sub-section of the input image
  */
IplImage* cropImage( IplImage* img, int x, int y, int w, int h );

/** Loads an image and converts it to normalized 32 bit floating point values
  * @param  fileName  - The path of the desired image file
  * @param  imgW      - The target width of the opened image
  * @param  imgH      - The target height of the opened image
  * @param  normalize - Specifies whether or not the image be normalized
  * @return A new IplImage with normalized 64 bit floating point values
  */
IplImage* loadImage64f( std::string fileName, int imgW=0, int imgH=0 );

/** Converts any IplImage to a 64 bit floating point single channel image
  * @param  src       - The source image
  * @param  imgW      - The target width of the opened image
  * @param  imgH      - The target height of the opened image
  * @return A new IplImage with normalized 64 bit floating point values
  */
IplImage* cvtTo64f1c( IplImage* src, int imgW=0, int imgH=0 );

/** Converts any IplImage to an 8 bit single channel image
  * @param  src       - The source image
  * @param  imgW      - The target width of the opened image
  * @param  imgH      - The target height of the opened image
  * @return A new 8 bit single channel IplImage
  */
IplImage* cvtTo8u1c( IplImage* src, int imgW=0, int imgH=0 );

/** Performs a bilateral filter on an image and returns the result
  * @param  src           - The source image
  * @param  spatialRadius - The spatial radius for the kernel
  * @param  colorRadius   - The color radius for the kernel
  * @param  iterations    - The number of times to perform the operation
  * @return A filtered version of the input image
  */
IplImage* bilateralSmooth( IplImage* src, int spatialRadius=9, int colorRadius=5, int iterations=3 );

/** Performs a Blackhat filter on an input image
  * @param  src        - The source image
  * @param  strelRad   - The radus of the structuring element
  * @param  iterations - The number of times to perform the operation
  * @return  A filtered version of the input image
  * @todo   Constrain parameters
  */
IplImage* blackHat( IplImage* src, int strelRad=5, int iterations=3 );

/** Performs an Erosion filter on an input image
  * @param  src - The source image
  * @param  strelRad - The radius for the erosion kernel
  * @param  iterations - The number of erosions to perorm
  * @return A filtered version of the input image
  */
IplImage* erode( IplImage* src, int strelRad=2, int iterations=1 );

enum CvLogicOp{ AND, OR, NOT, XOR, ADD, SUB, MUL, DIV };

/** Performs an smarth Thresholding filter on an input image
  * @param  src      - The source image
  * @param  meanBuff - fraction of stddev to set the threshold above the mean
  * @param  invert   - Specifies if the mask should be inverted
  * @return A filtered version of the input image
  */
IplImage* meanThreshold( IplImage* src, double meanBuff=0.25, bool invert=false );

IplImage* adaptiveThreshold( IplImage* src, int strelRad=7, double colorRadius=5, bool invert=false );

/** Produces a binary mask of the input image
  * @param  src       - The source image
  * @param  showSteps - Show the steps in masking the image
  * @return A binary mask version of the input image
  */
IplImage* starMask( IplImage* src, bool showSteps=false );

/** Determines if a feature window has content in it
  * @param  src - The image to test for content
  * @return A boolean indicating if the feature has content
  */
bool hasContent( IplImage* src );

IplImage* prep( IplImage* src );
IplImage* simp( IplImage* src );

void generateRects( int r0, std::vector<cv::Rect> &rects0, int r1, std::vector<cv::Rect> &rects1, int xC, int yC, int q );
void moveRects( int dx, int dy, std::vector<cv::Rect> &rects0, std::vector<cv::Rect> &rects1 );
void centerRects( int x, int y, std::vector<cv::Rect> &rects0, std::vector<cv::Rect> &rects1 );
void tester();


#ifdef WIN32
#define OS_SEP_STR "\\"
#define OS_SEP_CHAR '\\'
#else
#define OS_SEP_STR "/"
#define OS_SEP_CHAR '/'
#endif
