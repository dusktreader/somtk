#pragma once

#include <QString>
#include <QVector>

#include <algorithm>

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

#include "normalizer.h"
#include "som.h"
#include "suspect.h"
#include "somerror.h"
#include "classifier.h"

namespace hsom {

/// The SOM class provides an abstract base class for Self-Organizing Maps
class HSOM : public QThread
{
private:

    /// The normalizer used to normalize features
    NormalizerPtr normalizer;

    /// The SOM used by the hybrid
    SOMPtr som;

    /// The back-end classifier
    ClassifierPtr classifier;

    /// Extracts a list of features from a list of suspects
    QVector<FeaturePtr> extractFeatures(
            QVector<SuspectPtr> suspects /// A list of suspects from which to extract features
            );

    /// Generates the Histograms for all the training suspects
    void generateHistograms(
            QVector<SuspectPtr>& suspects ///< The list of suspects for which to generate histograms
            );

    /// Generates a Histogram for a training suspect assuming the SOM is fully trained
    void generateHistogram( SuspectPtr suspect ) const;



public:

    /// Constructs an empty Hybrid SOM
    HSOM(
        QObject* parent=NULL ///< The parent object for this HSOM ( inheriting from QThread )
        );

    /// Constructs the Hybrid-SOM and supplies a SOM and an ANN for it to use
    HSOM(
        NormalizerPtr normalizer, ///< The normalizer used to normalizer features for the SOM
        SOMPtr som,               ///< The SOM that this HSOM will use to build histograms of suspects
        ClassifierPtr classifier, ///< The back-end classifier used to identify incoming suspect's category
        QObject* parent=NULL      ///< The parent object for this HSOM ( inheriting from QThread )
        );

    /// Destructs the HSOM
    virtual ~HSOM(){}

    /// Clears the HSOM
    virtual void clear();

    /// Trains the HSOM
    void train(
        QVector<SuspectPtr>& trainingSuspects,       ///< The suspects to use for training
        QMap<QString, QVariant> normlizerParameters, ///< The calculation parameters to be used for the normalizer
        QMap<QString, QVariant> somParamters,        ///< The training parameters to be used for the som
        QMap<QString, QVariant> classifierParameters ///< The training parameters to be used for the classifier
        );

    /// Classifies suspects
    void classify(
            QVector<SuspectPtr>& suspects ///< The suspects to train with this HSOm
            );

    /// Classifies a single suspect
    void classify(
        SuspectPtr suspect ///< The suspect to classify
        );
};

} // namespace hsom
