#include "nullnormalizer.h"

namespace somtk {

NullNormalizer::NullNormalizer() : Normalizer(){}

NullNormalizer::~NullNormalizer(){}

void NullNormalizer::initialize( QMap<QString, QVariant> ){}

void NullNormalizer::calculate( QVector<FeaturePtr> ){}

void NullNormalizer::normalize( FeaturePtr ){}

void NullNormalizer::set( FeaturePtr feature )
{
    QVector<double>& v = *feature.data();
    for( int i=0; i<v.size(); i++ )
        v[i] = randomizer.randd();
}

} // namespace
