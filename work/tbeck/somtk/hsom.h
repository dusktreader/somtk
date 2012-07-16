#pragma once

#include <QString>
#include <QVector>

#include <algorithm>

#include "soms/som.h"
#include "normalizers/normalizer.h"
#include "suspects/suspect.h"
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

    /// Extracts a list of features from a list of suspects
    QVector<Feature> extractFeatures(
            QVector<SuspectPtr> suspects /// A list of suspects from which to extract features
            );

    /// Generates the Histograms for all the training suspects
    void generateHistograms(
            QVector<SuspectPtr> suspects ///< The list of suspects for which to generate histograms
            );

    /// Generates a Histogram for a training suspect assuming the SOM is fully trained
    void generateHistogram( SuspectPtr suspect );



public:

    /// Constructs the Hybrid-SOM and supplies a SOM and an ANN for it to use
    HSOM(
        SOMPtr som,               ///< The SOM that this HSOM will use to build histograms of suspects
        ClassifierPtr classifier, ///< The back-end classifier used to identify incoming suspect's category
        );

    /// Destructs the HSOM
    virtual ~HSOM(){}

    /// Trains the HSOM
    void train(
        QVector<SuspectPtr> trainingSuspects,        ///< The suspects to use for training
        QMap<QString, QVariant> somParamters,        ///< The training parameters to be used for the som
        QMap<QString, QVariant> classifierParameters ///< The training parameters to be used for the classifier
        );

    /// Classifies suspects
    void classify(
            QVector<SuspectPtr> suspects ///< The suspects to train with this HSOm
            );

    /// Classifies a single suspect
    void classify(
        SuspectPtr suspect ///< The suspect to classify
        );
};

} // namespace
