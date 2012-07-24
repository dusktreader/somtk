#include "suspect.h"

namespace somtk {

Suspect::Suspect( HistogramGrid gridTemplate )
{
    _predCategory = -1;
    _realCategory = -1;

    _histogram = HistogramPtr( new Histogram( gridTemplate ) );
}

QVector<FeaturePtr> Suspect::features()
{
    if( _features.empty() )
        generateFeatures();

    return _features;
}



void Suspect::setHistogram( HistogramPtr histogram )
{
    _histogram = histogram;
}



HistogramPtr Suspect::histogram()
{
    return _histogram;
}



void Suspect::setClassification( QVector<double> classification )
{
    _classification = classification;

    double maxCategory = DBL_MIN;
    for( int i = 0; i < _classification.size(); i++ )
    {
        if( _classification[i] > maxCategory )
        {
            maxCategory = _classification[i];
            _predCategory = i;
        }
    }
}



QVector<double> Suspect::classification()
{
    return _classification;
}



void Suspect::setRealCategory( int category )
{
    SOMError::requireCondition( category >= 0, "Attempted to set an invalid category");
    _realCategory = category;
}



int Suspect::realCategory()
{
    SOMError::requireCondition( _realCategory >= 0, "Real category hasn't been set yet.  Cannot fetch it" );
    return _realCategory;
}



int Suspect::predCategory()
{
    SOMError::requireCondition( _predCategory >= 0, "Classification hasn't been set yet.  Cannot fetch prediction" );
    return _predCategory;
}

} // namespace somtk
