#include "normalizer.h"

namespace hsom {

Normalizer::Normalizer() :
    _isCalculated( false )
{}




void Normalizer::calculate( QVector<Feature> features, QMap<QString, QVariant> calculationParameters )
{
    _calculationParameters = calculationParameters;
    calculateNormalizer( features, calculationParameters );
    _isCalculated = true;
}

} // namespace hsom
