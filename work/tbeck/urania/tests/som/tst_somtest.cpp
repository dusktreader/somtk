#include <QtCore/QString>
#include <QtTest/QtTest>



class SomTest : public QObject
{
    Q_OBJECT
    
public:
    SomTest();
    
private Q_SLOTS:
    void visualTest();
};

SomTest::SomTest()
{
}

void SomTest::visualTest()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SomTest)

#include "tst_somtest.moc"
