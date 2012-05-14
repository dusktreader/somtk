#include <QtCore/QString>
#include <QtTest/QtTest>

class SigmoidNormalizerTest : public QObject
{
    Q_OBJECT
    
public:
    SigmoidNormalizerTest();
    
private Q_SLOTS:
    void testCase1();
};

SigmoidNormalizerTest::SigmoidNormalizerTest()
{
}

void SigmoidNormalizerTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SigmoidNormalizerTest)

#include "tst_sigmoidnormalizertest.moc"
