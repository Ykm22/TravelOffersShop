/********************************************************************************
** Form generated from reading UI file 'TravelOffersShop.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAVELOFFERSSHOP_H
#define UI_TRAVELOFFERSSHOP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TravelOffersShopClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TravelOffersShopClass)
    {
        if (TravelOffersShopClass->objectName().isEmpty())
            TravelOffersShopClass->setObjectName(QString::fromUtf8("TravelOffersShopClass"));
        TravelOffersShopClass->resize(600, 400);
        menuBar = new QMenuBar(TravelOffersShopClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        TravelOffersShopClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TravelOffersShopClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        TravelOffersShopClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(TravelOffersShopClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        TravelOffersShopClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(TravelOffersShopClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        TravelOffersShopClass->setStatusBar(statusBar);

        retranslateUi(TravelOffersShopClass);

        QMetaObject::connectSlotsByName(TravelOffersShopClass);
    } // setupUi

    void retranslateUi(QMainWindow *TravelOffersShopClass)
    {
        TravelOffersShopClass->setWindowTitle(QCoreApplication::translate("TravelOffersShopClass", "TravelOffersShop", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TravelOffersShopClass: public Ui_TravelOffersShopClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAVELOFFERSSHOP_H
