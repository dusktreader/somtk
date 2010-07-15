#include "imagesuspect.h"

using namespace std;

ImageSuspect::ImageSuspect( IplImage* img, IplImage* msk,
                            int realCat, int catCt, int w, int h, int featW, int featH, string name ) :
Suspect( name, realCat, catCt, w, h ){
    ENTER;
    this->img = cvCloneImage( img );
    if( msk == NULL )
        this->msk = starMask( img );
    else
        this->msk = cvCloneImage( msk );
    imgX = 0;
    imgY = 0;
    this->featW = featW;
    this->featH = featH;
    RETURN;
}

ImageSuspect::~ImageSuspect(){
    ENTER;
    cvReleaseImage( &img );
    cvReleaseImage( &msk );
    RETURN;
}

void ImageSuspect::resetFeatureSz( int featW, int featH ){
    this->featW = featW;
    this->featH = featH;
}

Feature* ImageSuspect::getNextFeature(){
    ENTER;
    Feature* feat = NULL;
                                                                                                                        //IplImage* imgSub = NULL;
    IplImage* mskSub = NULL;
    while( imgX + featW < img->width && imgY + featH < img->height && feat == NULL ){
        mskSub = cropImage( msk, imgX, imgY, featW, featH );
        if( hasContent( mskSub ) ){
            /*
            imgSub = cropImage( img, imgX, imgY, featW, featH );
             */
            feat = new HuFeature( mskSub );
            /*
            feat = new HuFeature( imgSub );
            feat = new ImageFeature( featW, featH, imgSub );
            cvReleaseImage( &imgSub );
             */
        }
        cvReleaseImage( &mskSub );
        imgX += STEP_SIZE;
        if( imgX + featW >= img->width ){
            imgY += STEP_SIZE;
            imgX = imgY % STEP_SIZE;
        }
    }
    if( feat == NULL ){                                                                                                 // If there are no more features in the image, reset the scan coordinates
        imgX = 0;
        imgY = 0;
    }
    RETURN feat;
}

