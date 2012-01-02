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

Suspect::Suspect()
{}

Suspect::~Suspect()
{}

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

void Suspect::setClassification( const QVector<double>& classification )
{
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

void Suspect::setClassification( const QVector<double>& classification )
{
    SOMError::requireCondition( classification.size() == Suspect::categoryCount(),
                                "Invalid classification vector.  Output count is incorrect" );
    _classification = classification;
}

const QVector<double>& Suspect::classification() const
{
    return _classification;
}

int Suspect::realCategory()
{
    return _realCategory;
}

int Suspect::predCategory()
{
    return _predCategory;
}

} // namespace hsom
