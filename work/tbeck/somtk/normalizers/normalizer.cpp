#include "normalizer.h"

namespace somtk {

Normalizer::Normalizer() :
    _isCalculated( false )
{}



void Normalizer::calculate( QVector<FeaturePtr> features, QMap<QString, QVariant> calculationParameters )
{
    _calculationParameters = calculationParameters;
    calculateNormalizer( features );
    _isCalculated = true;
}


void Normalizer::normalizeAll( QVector<FeaturePtr> features )
{
    foreach( FeaturePtr feature, features )
        normalize( feature );
}

} // namespace
