#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QTextStream>

#include "tools/cvq/cvq.h"

class ToolsTest : public QObject
{
    Q_OBJECT

public:
    ToolsTest();

private Q_SLOTS:
    void cvqTest();
};

ToolsTest::ToolsTest()
{
}

void ToolsTest::cvqTest()
{
    QImage colorApple( "/data/code/data/apple_strawberry_library/apple_0.jpg" );
    cv::Mat_<double> cvImage = imageQrgb2CVdbl( colorApple );
    QImage grayApple = imageCVdbl2Qrgb( cvImage );
    grayApple.save( "/data/code/data/grayApple.jpg" );
    QVERIFY2( true, "Nothing" );
}

QTEST_APPLESS_MAIN(ToolsTest)

#include "tst_toolstest.moc"

