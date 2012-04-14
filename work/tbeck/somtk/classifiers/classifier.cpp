#include "classifier.h"

namespace somtk {

Classifier::Classifier() :
    _isTrained( false )
{}



void Classifier::readClassifierData( QDomElement& element )
{}



void Classifier::writeClassifierData( QDomElement& element )
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
    _trainingParameters = readVariantMap( element, "trainingParameters" );

    readClassifierData( element );
}



void Classifier::writeData( QDomElement& element )
{
    element.setAttribute( "isTrained", _isTrained ? "true" : "false" );
    writeVariantMap( element, "trainingParameters", _trainingParameters );

    writeClassifierData( element );
}

} // namespace
