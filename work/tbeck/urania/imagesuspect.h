#pragma once

#include "suspect.h"
#include "feature.h"
#include "hufeature.h"
#include "local.h"

#define STEP_SIZE 2

class ImageSuspect : public Suspect
{
private:
    IplImage* img;
    IplImage* msk;
    int imgX;
    int imgY;
    int featW;
    int featH;

public:
    ImageSuspect( IplImage* img, IplImage* msk, int realCat, int catCt, int w, int h, int featW, int featH, std::string name );
    virtual ~ImageSuspect();
    void resetFeatureSz( int featW, int featH );
    virtual Feature* getNextFeature();

};
