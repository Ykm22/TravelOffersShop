#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "MyListModel.h"
#include "srv.h"
#include "Observer.h"
#include "Qt_libraries.h"

class OfferCounter;

class WishlistGUI : public QWidget {
private:
	OfferStore& srv;
	QHBoxLayout* layoutMain;
	MyListModel* wishlistOffers;
	QListView* listView = new QListView;

	//adaugare
	QLabel* labelNameToAdd;
	QLineEdit* editAddWishlist;
	QPushButton* buttonAddWishlist;
	//generare random
	QLabel* labelGenerateWishlist;
	QLineEdit* editGenerateWishlist;
	QPushButton* buttonGenerateWishlist;
	//golire
	QPushButton* buttonEmptyWishlist;
	//export
	QLabel* labelExportWishlist;
	QLineEdit* editExportWishlist;
	QPushButton* buttonExportWishlist;

	//pt selectarea unei linii sa apara in line edit-uri
	QLabel* labelName;
	QLineEdit* nameText;
	QLabel* labelDestination;
	QLineEdit* destinationText;
	QLabel* labelType;
	QLineEdit* typeText;
	QLabel* labelPrice;
	QLineEdit* priceText;

	QPushButton* buttonClose;

	QPushButton* buttonLabelWindow;
	QPushButton* buttonTableWindow;
	QPushButton* buttonDrawWindow;

	void initGUIComponents();
	void connectSignalSlots();


	void AddToWishlist();
	void EmptyWishlist();
	void GenerateWishlist();
	void ExportWishlist();
	void reloadWishlist(vector<Offer> wishlist);
	void ShowLabelWindow();
	void ShowDrawWindow();
public:
	WishlistGUI(OfferStore& srv) :srv{ srv } {
		initGUIComponents();
		wishlistOffers = new MyListModel(srv.getWishlist().getWishlist());
		listView->setModel(wishlistOffers);
		connectSignalSlots();
	}
};

class OfferCounter : public QLabel, public Observer {
private:
	OfferWishlist& wishlist;
public:
	OfferCounter(OfferWishlist& wishlist) :wishlist{ wishlist } {
		setAttribute(Qt::WA_DeleteOnClose);
		wishlist.addObserver(this);
		update();
	}
	void update() override {
		setText("Wishlist-ul contine " + QString::number(wishlist.getWishlist().size()));
	}
	~OfferCounter() {
		wishlist.removeObserver(this);
	}
};

class OfferDraw : public QWidget, public Observer {
private:
	OfferWishlist& wishlist;
protected:
	void paintEvent(QPaintEvent* ev) override {
		int x = 10, y = 10, w = 10;
		QPainter g{ this };
		g.setPen(Qt::blue);
		for (const auto& offer : wishlist.getWishlist()) {
			g.drawRect(x, y, w, offer.getPrice() * 20);
			x += 40;
		}
	}
public:
	OfferDraw(OfferWishlist& wishlist) : wishlist{ wishlist } {
		update();
	}
	void update() override {
		this->repaint();
	}
};