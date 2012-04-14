#pragma once

#include <QMap>
#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>


#include "suspect.h"
#include "persist.h"

namespace somtk {

class Classifier : PersistXML
{
    Q_OBJECT

private:

    /// Indicates whether or not this classifier has been trained yet
    bool _isTrained;



protected:

    /// Describes the parameters used to tune the training of this classifier
    QMap<QString, QVariant> _trainingParameters;

    // The Classifier API

    /** @brief  Interfaces with the PersistXML API to ensure correct loading order for a classifier
     *  @note   This is optional.  If not implemented by a derived class, this function does nothing
     */
    virtual void readClassifierData( QDomElement& element );

    /** @brief  Interfaces with the PersistXML API to ensure correct saving order for a classifier
     *  @note   This is optional.  If not implemented by a derived class, this function does nothing
     */
    virtual void writeClassifierData( QDomElement& element );

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

    /// Classifies a single suspect
    virtual void classify(
            SuspectPtr suspect ///< The suspect to be classified
            ) = 0;



    // The PersistXML API.

    virtual void readData( QDomElement& element );
    virtual void writeData( QDomElement& element );
};

typedef QSharedPointer<Classifier> ClassifierPtr;

} // namespace
