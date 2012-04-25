#include "nullnormalizer.h"

namespace somtk {

NullNormalizer::NullNormalizer() : Normalizer(){}

void NullNormalizer::calculateNormalizer( QVector<FeaturePtr>, QMap<QString, QVariant> ){}

void NullNormalizer::normalize( FeaturePtr ){}

void NullNormalizer::setFeature( FeaturePtr feature )
{
    QVector<double>& v = *feature.data();
    for( int i=0; i<v.size(); i++ )
        v[i] = randomizer.randd();
}

} // namespace
