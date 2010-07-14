/********************************************************************************
** Form generated from reading UI file 'imageview.ui'
**
** Created: Tue Jul 13 16:43:39 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEW_H
#define UI_IMAGEVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageView
{
public:

    void setupUi(QWidget *ImageView)
    {
        if (ImageView->objectName().isEmpty())
            ImageView->setObjectName(QString::fromUtf8("ImageView"));
        ImageView->resize(400, 300);

        retranslateUi(ImageView);

        QMetaObject::connectSlotsByName(ImageView);
    } // setupUi

    void retranslateUi(QWidget *ImageView)
    {
        ImageView->setWindowTitle(QApplication::translate("ImageView", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImageView: public Ui_ImageView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEW_H
