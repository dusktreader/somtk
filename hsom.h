#pragma once

#include <QString>
#include <QVector>

#include <algorithm>

#include "soms/som.h"
#include "normalizers/normalizer.h"
#include "suspects/suspect.h"
#include "libraries/suspect_library.h"
#include "errors/somerror.h"
#include "classifiers/classifier.h"

namespace somtk {

class HSOM
{

private:

    /// The SOM used by the hybrid
    SOMPtr som;

    /// The back-end classifier
    ClassifierPtr classifier;

    /// Generates a Histogram for a training suspect assuming the SOM is fully trained
    void generateHistogram( SuspectPtr suspect );



public:

    /// Constructs the Hybrid-SOM and supplies a SOM and an ANN for it to use
    HSOM(
        SOMPtr som,              ///< The SOM that this HSOM will use to build histograms of suspects
        ClassifierPtr classifier ///< The back-end classifier used to identify incoming suspect's category
        );

    /// Destructs the HSOM
    virtual ~HSOM();

    /// Trains the HSOM
    void train(
        SuspectLibraryPtr       suspectLibrary,      ///< The library of suspects to train against
        QMap<QString, QVariant> somParamters,        ///< The training parameters to be used for the som
        QMap<QString, QVariant> classifierParameters ///< The training parameters to be used for the classifier
        );

    /// Classifies a single suspect
    void classify(
        SuspectPtr suspect ///< The suspect to classify
        );
};

} // namespace
