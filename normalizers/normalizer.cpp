#include "normalizer.h"

namespace somtk {

Normalizer::Normalizer() : _isCalculated( false ) {}

Normalizer::~Normalizer(){}



void Normalizer::calculateNormalizer( QVector<FeaturePtr> features )
{
    calculate( features );
    _isCalculated = true;
}



void Normalizer::normalizeAll( QVector<FeaturePtr> features )
{
    foreach( FeaturePtr feature, features )
        normalizeFeature( feature );
}



void Normalizer::normalizeFeature( FeaturePtr feature )
{
    SOMError::requireCondition( _isCalculated, "Can't normalize a feature with a noralizer that isn't calculated yet" );
    if( !feature->isNormalized() )
    {
        normalize( feature );
        feature->setNormalized();
    }
}



void Normalizer::setFeature( FeaturePtr feature )
{
    SOMError::requireCondition( _isCalculated, "Can't set feature with a normalizer that isn't calculated yet" );
    set( feature );
}

} // namespace
