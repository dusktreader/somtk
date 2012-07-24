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

} // namespace
