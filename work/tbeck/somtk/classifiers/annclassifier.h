#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

#include "classifier.h"

namespace somtk {

class ANNClassifier : public Classifier
{

private:

    /// The OpenCV ANN classifier object that this Classifier wraps
    CvANN_MLP* ann;

    /** @brief  The name of the file used to store and load the OpenCV data
     *  @note   This is just a temporary solution until i decide if I want to bytepack this into an xml file
     */
    QString fileName;



protected:

    // The Classifier API

    virtual void readClassifierData( QDomElement& element );
    virtual void writeClassifierData( QDomElement& element );
    virtual void trainClassifier( QVector<SuspectPtr> suspects, QMap<QString,QVariant> trainingParameters );



public:

    ANNClassifier( QString fileName );



    // The Classifier API

    virtual void classify( SuspectPtr suspect );
};

typedef QSharedPointer<ANNClassifier> ANNClassifierPtr;

} // namespace
