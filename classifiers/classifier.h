#pragma once

#include <QMap>
#include <QString>
#include <QVariant>
#include <QSharedPointer>

#include "suspects/suspect.h"
#include "libraries/suspect_library.h"

namespace somtk {

class Classifier
{

private:

    /// Indicates whether or not this classifier has been trained yet
    bool _isTrained;



protected:

    /// Describes the parameters used to tune the training of this classifier
    QMap<QString, QVariant> _trainingParameters;

    // The Classifier API

    /// Performs specific training algorithm for a classifier
    virtual void trainClassifier(
            SuspectLibraryPtr suspects,               ///< The library of suspects with which to train this classifier
            QMap<QString,QVariant> trainingParameters ///< The tuning parameters used to train this classifier
            ) = 0;

    /// Classifies a single suspect
    virtual void classifySuspect(
            SuspectPtr suspect ///< The suspect to be classified
            ) = 0;



public:

    /// Constructs a classifier base instance
    Classifier();

    /// Destroys a classifier
    virtual ~Classifier();

    /// Trains the classifier using a collection of known suspects
    void train(
            SuspectLibraryPtr suspectLibrary,         ///< The list of suspects with which to train this classifier
            QMap<QString,QVariant> trainingParameters ///< The tuning parameters used to train this classifier
            );

    /// Classifies a suspect
    void classify(
            SuspectPtr suspect ///< The suspect to be classified
            );
};

typedef QSharedPointer<Classifier> ClassifierPtr;

} // namespace
