#include "normalizer.h"

namespace hsom {

Normalizer::Normalizer() :
    _isCalculated( false )
{}



void Normalizer::readNormalizerData( QDomElement& element )
{}



void Normalizer::writeNormalizerData( QDomElement& element )
{}



void Normalizer::calculate( QVector<FeaturePtr> features, QMap<QString, QVariant> calculationParameters )
{
    _calculationParameters = calculationParameters;
    calculateNormalizer( features, calculationParameters );
    _isCalculated = true;
}



void Normalizer::readData( QDomElement& element )
{
    _isCalculated = element.attribute( "isCalculated" ) == "true";
    _calculationParameters = readVariantMap( element, "calculationParameters" );

    readClassifierData( element );
}



void Normalizer::writeData( QDomElement& element )
{
    element.setAttribute( "isCalculated", _isCalculated ? "true" : "false" );
    writeVariantMap( element, "calculationParameters", _calculationParameters );

    writeClassifierData( element );
}

} // namespace hsom
