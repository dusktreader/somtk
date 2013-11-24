#include "classifier.h"

namespace somtk {

Classifier::Classifier() :
    _isTrained( false )
{}

Classifier::~Classifier(){}



void Classifier::train( SuspectLibraryPtr suspectLibrary, QMap<QString, QVariant> trainingParameters )
{
    _trainingParameters = trainingParameters;
    trainClassifier( suspectLibrary, trainingParameters );
    _isTrained = true;
}



void Classifier::classify( SuspectPtr suspect )
{
    SOMError::requireCondition( _isTrained, "Can't classify: the classifier isn't trained yet" );
    classifySuspect( suspect );
}

} // namespace
