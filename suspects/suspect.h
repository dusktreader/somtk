#pragma once

#include <QSharedPointer>
#include <cfloat>

#include "tools/localassert.h"
#include "features/feature.h"
#include "histograms/histogram.h"

namespace somtk {

class Suspect
{

protected:

    /// The name of the suspect
    QString _name;

    /// The actual category of this suspect
    int _realCategory;

    /// The category that a classifier has predicted this suspect to be
    int _predCategory;

    /// A classification vector defining how this Suspect was classified
    QVector<double> _classification;

    /// The histogram produced for this suspect by a SOM
    HistogramPtr _histogram;

    /// The features embedded in this Suspect
    QVector<FeaturePtr> _features;

    /// Generates the features for this Suspect
    virtual void generateFeatures() = 0;



public:

    /// Construct a default suspect
    Suspect(
            HistogramGrid gridTemplate ///< A histogram grid template used to generate this suspect's histogram
            );

    /// Fetches a vector of the features embedded in this suspect
    QVector<FeaturePtr> features();

    /// Fetches the real category for this Suspect
    int realCategory();

    /// Sets the real category for this Suspect
    void setRealCategory( int realCategory );

    /// Fetches the predicted category for this Suspect
    int predCategory();

    /// Manually sets this suspects histogram to the supplied histogram
    void setHistogram(
            HistogramPtr histogram ///< The histogram to force
            );

    /// Fetches the histogram for this suspect
    HistogramPtr histogram();

    /// Sets the classification for this susect using a classification vector
    void setClassification(
            QVector<double> classification ///< A vector composed of binary flags indicating the classification
            );

    /// Fetches the classification vector for this suspect
    QVector<double> classification();

    /// Sets the name for this susect
    void setName(
            QString name ///< The name of the suspect
            );

    /// Fetches the name of this suspect
    QString name();
};

typedef QSharedPointer<Suspect> SuspectPtr;

} // namespace hsom
