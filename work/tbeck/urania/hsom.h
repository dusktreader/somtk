#pragma once

#include <QString>
#include <QVector>

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

#include "tools.hpp"
#include "cvtools.hpp"
#include "cvtypesplus.hpp"

#include "som.h"
#include "suspect.h"

namespace hsom {

/// The SOM class provides an abstract base class for Self-Organizing Maps
class HSOM : public QThread
{
private:

    /// The SOM used by the hybrid
    SOMPtr _som;

    /// The back-end Multilayer Perceptron Neural Network for classification
    CvANN_MLP* _ann;

    /// Trains the SOM component of the HSOM
    void trainSOM(
        int somEpochs,    ///< The number of training epochs to use
        double initAlpha, ///< The initial training weight
        double initR      ///< The intial radius of the training neighborhood
        );

    /// Generates the SOMHistograms needed for training the HSOM
    void generateHistograms();

    /// Trains the ANN component of the HSOM
    void trainANN(
        int annIters, ///< The number of iterations to use in training the ANN
        double annEps ///< The minimum adjustment size to use in training the ANN
        );

    /// @todo: doc
    QVector<double> normMean;
    QVector<double> normStdv;
    QVector<double> normAlpha;


protected:

    /// A list of suspects for training or classification
    QList<SusepctPtr> trainingSuspects;

    /** Checks if Performs a status check that may interface to an interactive control
      * @return A boolean indicating if processing should continue
      */
    virtual bool statusCheck(
        int iteration,       ///< The current iteration
        std::string msg1="", ///< An optional message
        std::string msg2="", ///< Another optional message
        int maxIters=0       ///< The maximum number of iterations to perform
        );

    /// Preprocessing for the suspects.  May be implemented by child classes.
    virtual void analyzeSuspects();

    /// Normalizes a list of features using a Sigmoid function to map elements between 0.0 and 1.0
    void normalizeFeatures( QList<FeaturePtr>& features, double epsilon=0.0625, int sigmaStep=2, unsigned maxFeatures=100000 );

    /// Updates the SOM with a single training suspect
    void updateSOM( SuspectPtr suspect );

    /// Updates the training suspect's histogram given a trained SOM
    void updateHistogram( Suspect* suspect ) const;

public:

    /// Constructs an empty Hybrid SOM
    HSOM(
        QObject* parent=NULL ///< The parent object for this HSOM ( inheriting from QThread )
        );

    /// Constructs the Hybrid-SOM and supplies a SOM and an ANN for it to use
    HSOM(
        SOMPtr som,          ///< The SOM that this HSOM will use to build histograms of suspects
        CvANN_MLP* ann,      ///< The ANN that this HSOM will use to classify histograms of suspects
        QObject* parent=NULL ///< The parent object for this HSOM ( inheriting from QThread )
        );

    /// Destructs the HSOM
    virtual ~HSOM();

    /// Clears the HSOM
    virtual void clear();

    /// Fetches the number of categories this HSOM can classify
    int catCt() const;

    /// Loads the suspects from a directory.  Implemented by derived classes
    virtual void loadSuspects(
        const QString& dirPath,     ///< The directory from which to fetch suspect images
        const QStringList& fileList ///< The list of image files to load
        ) = 0;

    /// Clears the suspect list
    void clearSuspects();

    /// Trains the Hybrid SOM
    void train(
        int somEpochs,    ///< The number of training epochs to use
        double initAlpha, ///< The initial training weight
        double initR,     ///< The inital radius of the training neighborhood
        int annIters,     ///< The number of iterations for ANN training
        double annEps     ///< The minimum adjustment size for ANN training
        );

    /// Report classification results
    double report(
        QString& text ///< The string that will contain the report
        );

    /// Classifies suspects
    void classify();

    /// Classifies a single suspect
    void classify(
        Suspect* suspect,        ///< The suspect to classify
        cv::Mat_<double>& input, ///< The input matrix for the ANN
        cv::Mat_<double>& output ///< The output matrix for the ANN
        );

    /// Saves an SOM to file
    void save( const QString& fileName );

    /// Loads an SOM from file
    void load( const QString& fileName );

};

} // namespace hsom
