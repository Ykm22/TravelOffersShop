#pragma once
#include "Qt_libraries.h"
#include "WishlistGUI.h"
#include "MyTableModel.h"
#include "srv.h"
#include <iostream>
#include <string>
#include <fstream>

using std::vector;
using std::string;

class GUI : public QWidget {
private:
	OfferStore& srv;
	WishlistGUI wishlistWindow{ srv };

	QWidget* left = new QWidget;
	QVBoxLayout* layoutLeft = new QVBoxLayout;
	QLabel* labelName = new QLabel{ "Nume oferta:" };
	QLabel* labelDestination = new QLabel{ "Destinatia ofertei:" };
	QLabel* labelType = new QLabel{ "Tipul ofertei:" };
	QLabel* labelPrice = new QLabel{ "Pretul ofertei:" };

	QLineEdit* editName;
	QLineEdit* editDestination;
	QLineEdit* editType;
	QLineEdit* editPrice;

	QPushButton* buttonAddOffer;
	QPushButton* buttonUpdateOffer;

	QLabel* labelNameDelete = new QLabel{ "Numele ofertei de sters:" };
	QLineEdit* editNameDelete;
	QPushButton* buttonRemoveOffer;

	QGroupBox* groupBox = new QGroupBox(tr("Tip sortare"));
	QRadioButton* radioSortAsc = new QRadioButton(QString::fromStdString("Crescator"));
	QRadioButton* radioSortDesc = new QRadioButton(QString::fromStdString("Descrescator"));
	QPushButton* buttonSort;

	QLabel* labelFilter = new QLabel{ "Destinatia dupa care se filtreaza:" };
	QLineEdit* editFilter;
	QPushButton* buttonFilter;

	QPushButton* buttonUndo;

	QPushButton* buttonReloadData;
	QTableWidget* tableOffers;
	//MyTableModel* tableOffers;
	//QTableView* tableView = new QTableView;

	//raport nr de tipuri
	map <string, int> M;
	map<QPushButton*, int> freqButtons;
	QVBoxLayout* layoutButtonDynamic = new QVBoxLayout;
	QWidget* buttonDynamic = new QWidget;
	vector<QPushButton*> buttons;
	vector<string> buttonTypes;
	map <QPushButton*, string> button_type;

	//pentru fereastra de wishlist
	QPushButton* buttonWishlist;

	/*QListWidget* wishlistOffers;
	QLineEdit* editAddWishlist;
	QPushButton* buttonAddWishlist;
	QPushButton* buttonEmptyWishlist;
	QLineEdit* nameText;
	QLineEdit* destinationText;
	QLineEdit* typeText;
	QLineEdit* priceText;

	QLineEdit* editGenerateWishlist;
	QPushButton* buttonGenerateWishlist;

	QPushButton* buttonExportWishlist;
	QLineEdit* editExportWishlist;*/

	void initGUIComponents();
	void connectSignalSlots();
	void reloadOfferList(vector<Offer> offers);
	void reloadWishlist(vector<Offer> offers);
public:
	GUI(OfferStore& srvOffers) :srv{ srvOffers } {
		initGUIComponents();
		//tableOffers = new MyTableModel(srv.getAllOffers());
		//tableView->setModel(tableOffers);
		connectSignalSlots();
		reloadOfferList(srv.getAllOffers());
		//reloadWishlist(srv.getWishlist().getWishlist());
		addReportTypeButtons();
	}
	void addReportTypeButtons();
	void guiAddOffer();
	void guiRemoveOffer();
	void guiUpdateOffer();
	void guiUndo();
	void guiShowWishlistWindow();
	/*void guiAddToWishlist();
	void guiEmptyWishlist();
	void guiGenerateWishlist();
	void guiExportWishlist();*/
};