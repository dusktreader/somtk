#include "normalizer.h"

namespace somtk {

Normalizer::Normalizer() :
    _isCalculated( false )
{}



void Normalizer::calculate( QVector<FeaturePtr> features, QMap<QString, QVariant> calculationParameters )
{
    _calculationParameters = calculationParameters;
    calculateNormalizer( features, calculationParameters );
    _isCalculated = true;
}

} // namespace hsom
