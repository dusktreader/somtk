#include "sigmoidnormalizer.h"

namespace hsom {

SigmoidNormalizer::SigmoidNormalizer() :
    Normalizer()
{}


void SigmoidNormalizer::calculateNormalizer( QVector<FeaturePtr> features, QMap<QString, QVariant> normalizerParameters )
{
    bool ok = true;

    ASSERT_MSG( normalizerParameters.contains( "epsilon" ), "Normalizer parameters do not contain an epsilon" );
    epsilon = normalizerParameters["epsilon"].toDouble( &ok );
    ASSERT_MSG( ok, "Couldn't convert epsilon value" );
    ASSERT_MSG( epsilon > 0, "Epsilon value must be greater than 0.0" );
    ASSERT_MSG( epsilon < 1.0, "Epsilon value must be less than 1.0" );

    ASSERT_MSG( normalizerParameters.contains( "sigmaStep" ), "Normalizer parameters do not include a sigma step" );
    sigmaStep = normalizerParameters["sigmaStep"].toInt( &ok );
    ASSERT_MSG( ok, "Couldn't convert sigma step" );
    ASSERT_MSG( sigmaStep > 0, "Sigma step must be greater than 0" );

    unsigned featureSize = features.first()->size();

    normMean  = QVector<double>( featureSize, 0.0 );
    normStdv  = QVector<double>( featureSize, 0.0 );
    normAlpha = QVector<double>( featureSize, 0.0 );

    foreach( FeaturePtr feature, features )
    {
        for( int i = 0; i < featureSize; i++ )
        {
            double t = feature->data()[i] - normMean[i];
            normMean[i] += t / featCt;
            normStdv[i] += t * ( feature->data()[i] - normMean[i] );
        }
    }

    for( int i = 0; i < featureSize; i++ )
    {
        normStdv[i] = sqrt( normStdv[i] / ( features.size() - 1 ) );
        normAlpha[i] = log( 1 / epsilon - 1 ) / ( sigmaStep * normStdv[i] );
    }
}



void SigmoidNormalizer::normalize( FeaturePtr feature )
{
    for( int i = 0; i < feature->size(); i++ )
    {
        double& t = feature->data()[i];
        t = 1 / ( 1 + exp( -normAlpha[i] * ( t - normMean[i] ) ) );
    }
}



void SigmoidNormalizer::readNormalizerData( QDomElement& element )
{
    bool ok = true;

    QVector<QVariant> persistVector;

    persistVector = PersistXML::readPersistanceVector( element, "mean" );
    normMean = QVector<double>( persistVector.size() );
    for( int i=0; i<persistVector.size(); i++ )
    {
        normMean[i] = persistVector[i].toDouble( &ok );
        ASSERT_MSG( ok, "Couldn't convert vector item" );
    }

    persistVector = PersistXML::readPersistanceVector( element, "stdv" );
    stdvMean = QVector<double>( persistVector.size() );
    for( int i=0; i<persistVector.size(); i++ )
    {
        stdvMean[i] = persistVector[i].toDouble( &ok );
        ASSERT_MSG( ok, "Couldn't convert vector item" );
    }

    persistVector = PersistXML::readPersistanceVector( element, "alpha" );
    normAlpha = QVector<double>( persistVector.size() );
    for( int i=0; i<persistVector.size(); i++ )
    {
        normAlpha[i] = persistVector[i].toDouble( &ok );
        ASSERT_MSG( ok, "Couldn't convert vector item" );
    }
}



void SigmoidNormalizer::writeNormalizerData( QDomElement& element )
{

    QVector<QVariant> persistVector;

    persistVector = QVector<QVariant>( normMean.size() );
    for( int i=0; i<normMean.size(); i++ )
        persistVector[i] = normMean[i];
    PersistXML::writePersistanceVector( element, "mean", persistVector );

    persistVector = QVector<QVariant>( normStdv.size() );
    for( int i=0; i<normStdv.size(); i++ )
        persistVector[i] = normStdv[i];
    PersistXML::writePersistanceVector( element, "stdv", persistVector );

    persistVector = QVector<QVariant>( normAlpha.size() );
    for( int i=0; i<normAlpha.size(); i++ )
        persistVector[i] = normAlpha[i];
    PersistXML::writePersistanceVector( element, "alpha", persistVector );

}

} // namespace hsom
