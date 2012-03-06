#include <QtCore/QString>
#include <QtTest/QtTest>

#include "som.h"

class SomTest : public QObject
{
    Q_OBJECT

public:
    SomTest();

private Q_SLOTS:
    void testCase1();
};

SomTest::SomTest()
{
}

void SomTest::testCase1()
{

    hsom::SOM colorSOM( QSize( 16, 16 ) );



    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SomTest)

#include "tst_somtest.moc"
