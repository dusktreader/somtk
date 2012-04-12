#include "nullnormalizer.h"

namespace somtk {

NullNormalizer::NullNormalizer() : Normalizer(){}

void NullNormalizer::calculateNormalizer( QVector<FeaturePtr>, QMap<QString, QVariant> ){}

void NullNormalizer::clear(){}

void NullNormalizer::normalize( FeaturePtr ){}

void NullNormalizer::setFeature( FeaturePtr feature )
{
    Feature& f = *feature.data();
    for( int i=0; i<f.size(); i++ )
        f[i] = randomizer.randd();
}

} // namespace hsom
