#include "nullnormalizer.h"

namespace somtk {

NullNormalizer::NullNormalizer() : Normalizer(){}

void NullNormalizer::calculateNormalizer( QVector<DVectorPtr>, QMap<QString, QVariant> ){}

void NullNormalizer::clear(){}

void NullNormalizer::normalize( DVectorPtr ){}

void NullNormalizer::setFeature( DVectorPtr vector )
{
    QVector<double>& v = *vector.data();
    for( int i=0; i<v.size(); i++ )
        v[i] = randomizer.randd();
}

} // namespace
