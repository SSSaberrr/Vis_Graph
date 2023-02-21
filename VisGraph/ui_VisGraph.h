/********************************************************************************
** Form generated from reading UI file 'VisGraph.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISGRAPH_H
#define UI_VISGRAPH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VisGraphClass
{
public:

    void setupUi(QWidget *VisGraphClass)
    {
        if (VisGraphClass->objectName().isEmpty())
            VisGraphClass->setObjectName(QString::fromUtf8("VisGraphClass"));
        VisGraphClass->resize(600, 400);

        retranslateUi(VisGraphClass);

        QMetaObject::connectSlotsByName(VisGraphClass);
    } // setupUi

    void retranslateUi(QWidget *VisGraphClass)
    {
        VisGraphClass->setWindowTitle(QCoreApplication::translate("VisGraphClass", "VisGraph", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VisGraphClass: public Ui_VisGraphClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISGRAPH_H
