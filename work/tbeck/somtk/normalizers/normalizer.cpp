#include "normalizer.h"

namespace somtk {

Normalizer::Normalizer() :
    _isCalculated( false )
{}



void Normalizer::calculate( QVector< QSharedPointer< QVector<double> > > vectors, QMap<QString, QVariant> calculationParameters )
{
    _calculationParameters = calculationParameters;
    calculateNormalizer( vectors, calculationParameters );
    _isCalculated = true;
}

} // namespace
