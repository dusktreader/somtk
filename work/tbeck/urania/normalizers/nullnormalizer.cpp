#include "nullnormalizer.h"

namespace hsom {

NullNormalizer::NullNormalizer() :
    Normalizer()
{}

void NullNormalizer::calculateNormalizer( QVector<Feature>, QMap<QString, QVariant> )
{}

void NullNormalizer::clear()
{}

void NullNormalizer::normalize( Feature& )
{}

void NullNormalizer::setFeature( Feature& feature )
{
    for( int i=0; i<feature.size(); i++ )
        feature[i] = randomizer.randd();
}

} // namespace hsom
