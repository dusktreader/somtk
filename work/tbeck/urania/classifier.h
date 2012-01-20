#pragma once

#include <QMap>
#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>


#include "suspect.h"
#include "persist.h"

namespace hsom {

class Classifier : PersistXML
{
    Q_OBJECT

private:

    /// Indicates whether or not this classifier has been trained yet
    bool _isTrained;

    /// Describes the parameters used to tune the training of this classifier
    QMap<QString, QVariant> _trainingParameters;



protected:

    // The Classifier API

    /// Interfaces with the PersistXML API to ensure correct loading order for a classifier
    virtual void readClassifierData( QDomElement& element ) = 0;

    /// Interfaces with the PersistXML API to ensure correct saving order for a classifier
    virtual void writeClassifierData( QDomElement& element ) = 0;

    /// Performs specific training algorithm for a classifier
    virtual void trainClassifier(
            QVector<SuspectPtr> suspects,             ///< The list of suspects with which to train this classifier
            QMap<QString,QVariant> trainingParameters ///< The tuning parameters used to train this classifier
            ) = 0;



public:

    /// Constructs a classifier base instance
    Classifier();

    /// Trains the classifier using a collection of known suspects
    void train(
            QVector<SuspectPtr> suspects,             ///< The list of suspects with which to train this classifier
            QMap<QString,QVariant> trainingParameters ///< The tuning parameters used to train this classifier
            );



    // The Classifier API

    virtual void classify(
            SuspectPtr suspect ///< The suspect to be classified
            ) = 0;



    // The PersistXML API.

    virtual void readData( QDomElement& element );
    virtual void writeData( QDomElement& element );
};

typedef QSharedPointer<Classifier> ClassifierPtr;

} // namespace hsom
