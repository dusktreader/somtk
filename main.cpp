#include <QDebug>
#include <QString>
#include <QImageReader>

#include "grids/fasthexgrid.hpp"
#include "normalizers/sigmoidnormalizer.h"
#include "suspects/colorsuspect.h"
#include "soms/som.h"
#include "classifiers/annclassifier.h"
#include "libraries/library.h"

#include "hsom.h"

using namespace somtk;

int main()
{
    qDebug() << "Starting HSOM test executable" << endl;
    QVector<int> gridSize;
    gridSize << 25;

    qDebug() << "Creating gridTemplate for histograms" << endl;
    HistogramGrid gridTemplate( new FastHexGrid<double>( gridSize ) );

    qDebug() << "Creating Training Library" << endl;
    Library trainingLibrary;
    trainingLibrary.load( "/home/dusktreader/image_library/training_library.xml", gridTemplate );

    qDebug() << "Creating Testing Library" << endl;
    Library testingLibrary;
    testingLibrary.load( "/home/dusktreader/image_library/testing_library.xml", gridTemplate );

    qDebug() << "Creating and initializing Normalizer" << endl;
    NormalizerPtr normalizer( new SigmoidNormalizer() );
    QMap<QString, QVariant> normalizerParameters;
    normalizerParameters["epsilon"]   = 1.0e-6;
    normalizerParameters["sigmaStep"] = 5;
    normalizer->initialize( normalizerParameters );

    qDebug() << "Creating and initializing SOM" << endl;
    FeatureGrid somGrid( new FastHexGrid<FeaturePtr>( gridSize ) );
    SOMPtr som( new SOM( somGrid, normalizer ) );
    
    qDebug() << "Creating and initializing Classifier" << endl;
    ClassifierPtr classifier( new ANNClassifier() );

    QMap<QString, QVariant> somParameters;
    somParameters["maxEpochs"] = 20;
    somParameters["initialAlpha"] = 0.8;
    somParameters["initialRadiusRatio"] = 0.40;

    QMap<QString, QVariant> classifierParameters;
    classifierParameters["inputWidth"] = somGrid->capacity();
    classifierParameters["outputWidth"] = trainingLibrary.categoryCount();

    qDebug() << "Creating and initializing HSOM" << endl;
    HSOM hsom( som, classifier );
    qDebug() << "Training HSOM" << endl;
    hsom.train( trainingLibrary.suspects(), somParameters, classifierParameters );
    qDebug() << "Testing HSOM" << endl;
    hsom.classify( testingLibrary.suspects() );

    foreach( SuspectPtr suspect, testingLibrary.suspects() )
    {
        qDebug() << suspect->name() << ":";
        foreach( double d, suspect->classification() )
            qDebug() << d << " ";
        qDebug() << ":" << suspect->predCategory() << endl;
    }

    return 0;
}
