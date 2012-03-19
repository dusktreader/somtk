#include "nullnormalizer.h"

namespace hsom {

NullNormalizer::NullNormalizer() :
    Normalizer()
{}

void NullNormalizer::calculateNormalizer( QVector<Feature> features, QMap<QString, QVariant> normalizerParameters )
{}

void NullNormalizer::normalize( Feature& feature )
{}

void NullNormalizer::setFeature( Feature& feature )
{
    for( int i=0; i<feature.size(); i++ )
        feature[i] = randomizer.randd();
}

void NullNormalizer::clear()
{}

} // namespace hsom
