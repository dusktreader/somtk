/********************************************************************************
** Form generated from reading UI file 'megaslider.ui'
**
** Created: Tue Jul 13 16:44:03 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEGASLIDER_H
#define UI_MEGASLIDER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MegaSlider
{
public:
    QGridLayout *gridLayout;
    QLabel *sliderLabel;
    QSlider *slider;
    QFrame *frame;
    QLabel *number;
    QToolButton *leftButton;
    QToolButton *rightButton;
    QToolButton *minButton;
    QToolButton *maxButton;

    void setupUi(QWidget *MegaSlider)
    {
        if (MegaSlider->objectName().isEmpty())
            MegaSlider->setObjectName(QString::fromUtf8("MegaSlider"));
        MegaSlider->resize(498, 257);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MegaSlider->sizePolicy().hasHeightForWidth());
        MegaSlider->setSizePolicy(sizePolicy);
        MegaSlider->setMinimumSize(QSize(100, 60));
        MegaSlider->setAutoFillBackground(false);
        gridLayout = new QGridLayout(MegaSlider);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        sliderLabel = new QLabel(MegaSlider);
        sliderLabel->setObjectName(QString::fromUtf8("sliderLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sliderLabel->sizePolicy().hasHeightForWidth());
        sliderLabel->setSizePolicy(sizePolicy1);
        sliderLabel->setMinimumSize(QSize(70, 20));
        sliderLabel->setFrameShape(QFrame::NoFrame);
        sliderLabel->setScaledContents(false);
        sliderLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(sliderLabel, 0, 0, 1, 5);

        slider = new QSlider(MegaSlider);
        slider->setObjectName(QString::fromUtf8("slider"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(90);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(slider->sizePolicy().hasHeightForWidth());
        slider->setSizePolicy(sizePolicy2);
        slider->setMinimumSize(QSize(0, 20));
        slider->setMaximum(1000);
        slider->setSingleStep(0);
        slider->setPageStep(0);
        slider->setOrientation(Qt::Horizontal);
        slider->setTickPosition(QSlider::NoTicks);
        slider->setTickInterval(0);

        gridLayout->addWidget(slider, 1, 2, 1, 1);

        frame = new QFrame(MegaSlider);
        frame->setObjectName(QString::fromUtf8("frame"));
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMinimumSize(QSize(0, 20));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        number = new QLabel(frame);
        number->setObjectName(QString::fromUtf8("number"));
        number->setGeometry(QRect(50, 0, 71, 21));
        number->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(frame, 2, 0, 1, 5);

        leftButton = new QToolButton(MegaSlider);
        leftButton->setObjectName(QString::fromUtf8("leftButton"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(5);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(leftButton->sizePolicy().hasHeightForWidth());
        leftButton->setSizePolicy(sizePolicy3);
        leftButton->setMinimumSize(QSize(10, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/left"), QSize(), QIcon::Normal, QIcon::Off);
        leftButton->setIcon(icon);

        gridLayout->addWidget(leftButton, 1, 1, 1, 1);

        rightButton = new QToolButton(MegaSlider);
        rightButton->setObjectName(QString::fromUtf8("rightButton"));
        sizePolicy3.setHeightForWidth(rightButton->sizePolicy().hasHeightForWidth());
        rightButton->setSizePolicy(sizePolicy3);
        rightButton->setMinimumSize(QSize(10, 20));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/right"), QSize(), QIcon::Normal, QIcon::Off);
        rightButton->setIcon(icon1);

        gridLayout->addWidget(rightButton, 1, 3, 1, 1);

        minButton = new QToolButton(MegaSlider);
        minButton->setObjectName(QString::fromUtf8("minButton"));
        sizePolicy3.setHeightForWidth(minButton->sizePolicy().hasHeightForWidth());
        minButton->setSizePolicy(sizePolicy3);
        minButton->setMinimumSize(QSize(10, 20));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/begin"), QSize(), QIcon::Normal, QIcon::Off);
        minButton->setIcon(icon2);

        gridLayout->addWidget(minButton, 1, 0, 1, 1);

        maxButton = new QToolButton(MegaSlider);
        maxButton->setObjectName(QString::fromUtf8("maxButton"));
        sizePolicy3.setHeightForWidth(maxButton->sizePolicy().hasHeightForWidth());
        maxButton->setSizePolicy(sizePolicy3);
        maxButton->setMinimumSize(QSize(10, 20));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/end"), QSize(), QIcon::Normal, QIcon::Off);
        maxButton->setIcon(icon3);

        gridLayout->addWidget(maxButton, 1, 4, 1, 1);


        retranslateUi(MegaSlider);

        QMetaObject::connectSlotsByName(MegaSlider);
    } // setupUi

    void retranslateUi(QWidget *MegaSlider)
    {
        MegaSlider->setWindowTitle(QApplication::translate("MegaSlider", "Form", 0, QApplication::UnicodeUTF8));
        sliderLabel->setText(QApplication::translate("MegaSlider", "Label", 0, QApplication::UnicodeUTF8));
        number->setText(QApplication::translate("MegaSlider", "0", 0, QApplication::UnicodeUTF8));
        leftButton->setText(QString());
        rightButton->setText(QString());
        minButton->setText(QApplication::translate("MegaSlider", "...", 0, QApplication::UnicodeUTF8));
        maxButton->setText(QApplication::translate("MegaSlider", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MegaSlider: public Ui_MegaSlider {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEGASLIDER_H
