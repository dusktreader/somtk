#pragma once

#include <QString>
#include <QVector>

#include <algorithm>

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

#include "som.h"
#include "suspect.h"
#include "somerror.h"

namespace hsom {

/// The SOM class provides an abstract base class for Self-Organizing Maps
class HSOM : public QThread
{
private:

    /// The SOM used by the hybrid
    SOMPtr _som;

    /// The back-end Multilayer Perceptron Neural Network for classification
    CvANN_MLP* _ann;

    /// Extracts a list of features from a list of suspects
    QVector<FeaturePtr> extractFeatures(
            QVector<SuspectPtr>& suspects /// A list of suspects from which to extract features
            );

    /// Normalizes a list of features using a Sigmoid function to map elements between 0.0 and 1.0
    void calculateFeatureNormalization( QVector<FeaturePtr>& features, double epsilon=0.0625, int sigmaStep=2, unsigned maxFeatures=100000 );

    /// Normalizes a list of features using the computed normalization function parameters
    void normalizeFeatures( QVector<FeaturePtr>& features );

    /// Normalizes a single feature using the computed normalization function parameters
    void normalizeFeature( FeaturePtr feature );

    /// Trains the SOM component of the HSOM
    void trainSOM(
        QVector<FeaturePtr>& features, ///< The list of features with which to train the SOM
        int    epochCount,             ///< The number of training epochs to use
        double initialAlpha,           ///< The initial training weight
        double initialRadiusRatio      ///< The intial radius of the training neighborhood
        );

    /// Generates the Histograms for all the training suspects
    void generateHistograms(
            QVector<SuspectPtr>& suspects ///< The list of suspects for which to generate histograms
            );

    /// Generates a Histogram for a training suspect assuming the SOM is fully trained
    void generateHistogram( SuspectPtr suspect ) const;

    /// Trains the ANN component of the HSOM
    void trainANN(
            const QVector<SuspectPtr>& suspects, ///< The list of suspects with which to train the ANN
            int annIterations,                   ///< The number of iterations to use in training the ANN
            double annEps                        ///< The minimum adjustment size to use in training the ANN
        );

    /// Statistical metrics used for the normalization function
    QVector<double> normMean;
    QVector<double> normStdv;
    QVector<double> normAlpha;



protected:

    /** Checks if Performs a status check that may interface to an interactive control
      * @return A boolean indicating if processing should continue
      */
    virtual bool statusCheck(
        int iteration,       ///< The current iteration
        std::string msg1="", ///< An optional message
        std::string msg2="", ///< Another optional message
        int maxIters=0       ///< The maximum number of iterations to perform
        );





public:

    /// Constructs an empty Hybrid SOM
    HSOM(
        QObject* parent=NULL ///< The parent object for this HSOM ( inheriting from QThread )
        );

    /// Constructs the Hybrid-SOM and supplies a SOM and an ANN for it to use
    HSOM(
        SOMPtr som,          ///< The SOM that this HSOM will use to build histograms of suspects
        QObject* parent=NULL ///< The parent object for this HSOM ( inheriting from QThread )
        );

    /// Destructs the HSOM
    virtual ~HSOM();

    /// Clears the HSOM
    virtual void clear();

    /// Trains the HSOM
    void train(
        QVector<SuspectPtr>& trainingSuspects, ///< The suspects to use for training
        int epochCout,                         ///< The number of training epochs to use
        double initialAlpha,                   ///< The initial training weight
        double initRadiusRatio,                ///< The inital radius of the training neighborhood
        int annIterations,                     ///< The number of iterations for ANN training
        double annEps                          ///< The minimum adjustment size for ANN training
        );

    /// Classifies suspects
    void classify(
            QVector<SuspectPtr>& suspects ///< The suspects to train with this HSOm
            );

    /// Classifies a single suspect
    void classify(
        SuspectPtr suspect, ///< The suspect to classify
        );
};

} // namespace hsom
