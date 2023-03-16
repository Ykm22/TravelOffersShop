#pragma once
#include <QAbstractTableModel>
#include "Offer.h"
#include <vector>
#include "qbrush.h"
#include "qdebug.h"

class MyTableModel : public QAbstractTableModel {
private:
	std::vector<Offer>offers;
public:
	MyTableModel(const std::vector<Offer> offers) :offers{ offers } {};

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		qDebug() << "intrat " << offers.size();
		return  offers.size();
	}
	int columnCount(const QModelIndex& parent = QModelIndex()) const override {
		return 4;
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
		if (role == Qt::ForegroundRole) {
			Offer o = offers[index.row()];
			if (o.getPrice() < 50) {
				return QBrush{ Qt::red };
			}
		}
		if (role == Qt::DisplayRole) {
			Offer o = offers[index.row()];
			if (index.column() == 0)
				return QString::fromStdString(o.getName());
			if (index.column() == 1)
				return QString::fromStdString(o.getDestination());
			if (index.column() == 2)
				return QString::fromStdString(o.getType());
			if (index.column() == 3)
				return QString::number(o.getPrice());
		}
		return QVariant{};
	}

	void setOffers(const std::vector<Offer> offers) {
		this->offers = offers;
		auto topLeft = createIndex(0, 0);
		auto bottomRight = createIndex(rowCount(), columnCount());
		emit dataChanged(topLeft, bottomRight);
	}
};