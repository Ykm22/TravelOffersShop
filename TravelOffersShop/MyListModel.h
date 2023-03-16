#pragma once
#include <QAbstractListModel>
#include "Offer.h"
#include <vector>
#include "qcolor.h"

using namespace std;

class MyListModel : public QAbstractListModel {
	std::vector<Offer> offers;
public:
	MyListModel(const std::vector<Offer>& offers) :offers{ offers } {};

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return offers.size();
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
		if (role == Qt::DisplayRole) {
			auto destination = offers[index.row()].getDestination();
			return QString::fromStdString(destination);
		}
		if (role == Qt::UserRole) {
			auto type = offers[index.row()].getType();
			return QString::fromStdString(type);
		}
		if (role == Qt::BackgroundRole) {
			if (offers[index.row()].getPrice() < 100) {
				return QColor(Qt::red);
			}
		}
		return QVariant{};
	}
	void setOffers(const std::vector<Offer>& offers) {
		this->offers = offers;
		auto topLeft = createIndex(0, 0);
		auto bottomRight = createIndex(rowCount(), 1);
		emit dataChanged(topLeft, bottomRight);
	}
};