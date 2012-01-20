#include "classifier.h"

Classifier::Classifier() :
    _isTrained( false )
{}



void Classifier::train( QVector<SuspectPtr> suspects, QMap<QString, QVariant> trainingParameters )
{
    _trainingParameters = trainingParameters;
    trainClassifier( suspects, trainingParameters );
    _isTrained = true;
}



void Classifier::readData( QDomElement& element )
{
    _isTrained = element.attribute( "isTrained" ) == "true";
    _trainingParameters = readPersistanceMap( element, "trainingParameters" );

    readClassifierData( element );
}



void Classifier::writeData( QDomElement& element )
{
    element.setAttribute( "isTrained", _isTrained ? "true" : "false" );
    writePersistanceMap( element, "trainingParameters", _trainingParameters );

    writeClassifierData( element );
}
