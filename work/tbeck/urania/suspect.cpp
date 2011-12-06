#include "suspect.h"

namespace hsom {

Suspect::Suspect()
{}

Suspect::~Suspect()
{}

QList<FeaturePtr> Suspect::features()
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

const QVector<double> Suspect::classification() const
{
    return _cats;
}

int Suspect::realCategory()
{
    return _realCategory;
}

int Suspect::predCategory()
{
    return _predCategory;
}

void Suspect::incrementHistogram( int idx )
{
    histogram.increment( idx );
}

void Suspect::incrementHistogram( const PointPlus<int>& pt )
{
    histogram.increment( pt );
}

void Suspect::normalizeHistogram()
{
    histogram.normalize();
}

cv::Mat Suspect::vizHistogram()
{
    return histogram.vizHistogram();
}

} // namespace hsom
