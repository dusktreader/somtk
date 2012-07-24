#include "classifier.h"

namespace somtk {

Classifier::Classifier() :
    _isTrained( false )
{}



void Classifier::train( QVector<SuspectPtr> suspects, QMap<QString, QVariant> trainingParameters )
{
    _trainingParameters = trainingParameters;
    trainClassifier( suspects, trainingParameters );
    _isTrained = true;
}



void Classifier::classify( SuspectPtr suspect )
{
    SOMError::requireCondition( _isTrained, "Can't classify: the classifier isn't trained yet" );
    classifySuspect( suspect );
}

} // namespace
