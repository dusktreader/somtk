#include "suspect.h"

namespace hsom {

int Suspect::_categoryCount = -1;

void Suspect::setCatergoryCount( int count )
{
    _categoryCount = count;
}

int Suspect::categoryCount()
{
    return _categoryCount;
}

Suspect::Suspect() :
    _realCategory( -1 ),
    _predCategory( -1 )
{}

Suspect::~Suspect()
{}

QVector<Feature> Suspect::features()
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

void Suspect::setClassification( const QVector<double>& classification )
{
    SOMError::requireCondition( classification.size() == Suspect::categoryCount(),
                                "Invalid classification vector.  Output count is incorrect" );

    _classification = classification;
    double maxCategory;
    for( unsigned i=0; i_<classification.size(); i++ )
    {
        if( _classification[i] > maxCategory )
        {
            maxCategory = _classification[i];
            _predCategory = i;
        }
    }
}

const QVector<double>& Suspect::classification() const
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

} // namespace hsom
