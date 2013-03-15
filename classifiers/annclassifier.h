#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

#include "classifiers/classifier.h"

namespace somtk {

class ANNClassifier : public Classifier
{

private:

    /// The OpenCV ANN classifier object that this Classifier wraps
    CvANN_MLP* _ann;

    /// The input width for the ANN
    int inputWidth;

    /// The output width for the ANN
    int outputWidth;



protected:

    // The Classifier API
    virtual void trainClassifier( QVector<SuspectPtr> suspects, QMap<QString,QVariant> trainingParameters );
    virtual void classifySuspect( SuspectPtr suspect );



public:

    ANNClassifier();
};

typedef QSharedPointer<ANNClassifier> ANNClassifierPtr;

} // namespace
