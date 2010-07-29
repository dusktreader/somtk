#pragma once

#include "traindialog.h"
#include "qimagehsom.h"
#include "local.h"

#ifdef WIN32
#define TRAIN_DIR "/Documents and Settings/d3x874/My Documents/suspect_library/train"
#define SOM_DIR "/Documents and Settings/d3x874/My Documents/suspect_library/soms"
#else
#define SOM_DIR "/data/suspect_library/soms"
#define TRAIN_DIR "/data/suspect_library/train"
#endif

/** Converts an IplImage to a QPixmap image
  * @param  ipl:   The IplImage to convert
  * @param  size:  The size of the target QPixmap ( No resize if NULL )
  * @param  tmode: The transformation mode for resizing
  * @param  amode: The aspect ratio conversion mode for resizing
  */
QPixmap iplToQPixmap( IplImage* ipl,
                      QSize* size=NULL,
                      Qt::TransformationMode tmode=Qt::SmoothTransformation,
                      Qt::AspectRatioMode amode=Qt::KeepAspectRatio );
