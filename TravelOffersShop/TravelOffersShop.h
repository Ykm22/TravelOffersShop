#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TravelOffersShop.h"

class TravelOffersShop : public QMainWindow
{
    Q_OBJECT

public:
    TravelOffersShop(QWidget *parent = nullptr);
    ~TravelOffersShop();

private:
    Ui::TravelOffersShopClass ui;
};
