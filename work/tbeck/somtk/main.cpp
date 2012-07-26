#include <QDebug>
#include <QString>
#include <QImageReader>

#include "grids/fasthexgrid.hpp"
#include "normalizers/sigmoidnormalizer.h"
#include "suspects/colorsuspect.h"
#include "soms/som.h"
#include "classifiers/annclassifier.h"
#include "library.h"
#include "hsom.h"

using namespace somtk;

int main(int argc, char *argv[])
{
    QVector<int> gridSize;
    gridSize << 25;

    HistogramGrid gridTemplate( new FastHexGrid<double>( gridSize ) );

    Library trainingLibrary;
    trainingLibrary.load( "/data/code/data/apple_strawberry_library/training_library.xml", gridTemplate );

    Library testingLibrary;
    testingLibrary.load( "/data/code/data/apple_strawberry_library/testing_library.xml", gridTemplate );

    NormalizerPtr normalizer( new SigmoidNormalizer() );
    QMap<QString, QVariant> normalizerParameters;
    normalizerParameters["epsilon"]   = 1.0e-6;
    normalizerParameters["sigmaStep"] = 5;
    normalizer->initialize( normalizerParameters );

    FeatureGrid somGrid( new FastHexGrid<FeaturePtr>( gridSize ) );
    SOMPtr som( new SOM( somGrid, normalizer ) );
    ClassifierPtr classifier( new ANNClassifier() );

    QMap<QString, QVariant> somParameters;
    somParameters["maxEpochs"] = 20;
    somParameters["initialAlpha"] = 0.8;
    somParameters["initialRadiusRatio"] = 0.40;

    QMap<QString, QVariant> classifierParameters;
    classifierParameters["inputWidth"] = somGrid->capacity();
    classifierParameters["outputWidth"] = trainingLibrary.categoryCount();

    HSOM hsom( som, classifier );
    hsom.train( trainingLibrary.suspects(), somParameters, classifierParameters );
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
