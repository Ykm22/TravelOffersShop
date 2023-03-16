#include "GUI.h"
#include <assert.h>
#include "qslider.h"

void GUI::initGUIComponents() {
	//main layout
	QHBoxLayout* layoutMain = new QHBoxLayout;
	this->setLayout(layoutMain);

	//partea stanga	
	//layout vertical
	left->setLayout(layoutLeft);

	//comp pt add - QFormLayout
	QWidget* form = new QWidget;
	QFormLayout* layoutForm = new QFormLayout;
	form->setLayout(layoutForm);
	editName = new QLineEdit;
	editDestination = new QLineEdit;
	editType = new QLineEdit;
	editPrice = new QLineEdit;

	layoutForm->addRow(labelName, editName);
	layoutForm->addRow(labelDestination, editDestination);
	layoutForm->addRow(labelType, editType);
	layoutForm->addRow(labelPrice, editPrice);
	buttonAddOffer = new QPushButton("Adauga oferta");
	layoutForm->addWidget(buttonAddOffer);
	buttonUpdateOffer = new QPushButton("Update oferta");
	layoutForm->addWidget(buttonUpdateOffer);

	//punem formul construit acum in layout-ul din stanga
	layoutLeft->addWidget(form);

	QWidget* formDelete = new QWidget;
	QFormLayout* layoutFormDelete = new QFormLayout;
	formDelete->setLayout(layoutFormDelete);
	editNameDelete = new QLineEdit;
	layoutFormDelete->addRow(labelNameDelete, editNameDelete);
	buttonRemoveOffer = new QPushButton("Sterge oferta");
	layoutFormDelete->addWidget(buttonRemoveOffer);
	layoutLeft->addWidget(formDelete);

	//GroupBox pt sortare
	QVBoxLayout* layoutRadioBox = new QVBoxLayout;
	this->groupBox->setLayout(layoutRadioBox);
	layoutRadioBox->addWidget(radioSortAsc);
	layoutRadioBox->addWidget(radioSortDesc);

	buttonSort = new QPushButton("Sortare oferte dupa destinatie");
	layoutRadioBox->addWidget(buttonSort);

	//in partea stanga
	layoutLeft->addWidget(groupBox);

	//form pt filtrare dupa destinatie
	QWidget* formFilter = new QWidget;
	QFormLayout* layoutFormFilter = new QFormLayout;
	formFilter->setLayout(layoutFormFilter);
	editFilter = new QLineEdit;
	layoutFormFilter->addRow(labelFilter, editFilter);
	buttonFilter = new QPushButton("Filtrare dupa destinatie");
	layoutFormFilter->addWidget(buttonFilter);
	layoutLeft->addWidget(formFilter);

	//buton pt reincarcare date
	buttonUndo = new QPushButton("Undo");
	layoutLeft->addWidget(buttonUndo);
	buttonReloadData = new QPushButton("Reload data");
	layoutLeft->addWidget(buttonReloadData);

	//right - doar tabel cu oferte
	QWidget* right = new QWidget;
	QVBoxLayout* layoutRight = new QVBoxLayout;
	right->setLayout(layoutRight);

	int noLines = 10, noColumns = 4;
	this->tableOffers = new QTableWidget{ noLines, noColumns };

	//header-ul tabelului
	QStringList tableHeaderList;
	tableHeaderList << "Name" << "Destination" << "Type" << "Price";
	tableOffers->setHorizontalHeaderLabels(tableHeaderList);
	tableOffers->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	buttonDynamic->setLayout(layoutButtonDynamic);

	layoutMain->addWidget(left);
	QLabel* offersText = new QLabel("Oferte");
	layoutRight->addWidget(offersText);

	buttonWishlist = new QPushButton("Open wishlist");
	layoutRight->addWidget(buttonWishlist);


	layoutRight->addWidget(tableOffers);

	layoutMain->addWidget(right);
	layoutMain->addWidget(buttonDynamic);
}
void GUI::connectSignalSlots() {
	QObject::connect(buttonAddOffer, &QPushButton::clicked, this, &GUI::guiAddOffer);
	QObject::connect(buttonUpdateOffer, &QPushButton::clicked, this, &GUI::guiUpdateOffer);
	QObject::connect(buttonRemoveOffer, &QPushButton::clicked, this, &GUI::guiRemoveOffer);
	QObject::connect(buttonSort, &QPushButton::clicked, [&]() {
		if (this->radioSortAsc->isChecked()) {
			this->reloadOfferList(srv.sortDestination(1));
		}
		else if (this->radioSortDesc->isChecked())
			this->reloadOfferList(srv.sortDestination(0));
		});

	QObject::connect(buttonFilter, &QPushButton::clicked, [&]() {
		string filterCriteria = this->editFilter->text().toStdString();
		this->reloadOfferList(srv.filterDestination(filterCriteria));
		});
	QObject::connect(buttonUndo, &QPushButton::clicked, this, &GUI::guiUndo);
	QObject::connect(buttonReloadData, &QPushButton::clicked, [&]() {
		this->reloadOfferList(srv.getAllOffers());
		});
	QObject::connect(buttonWishlist, &QPushButton::clicked, this, &GUI::guiShowWishlistWindow);
	QObject::connect(tableOffers, &QTableWidget::itemSelectionChanged, [=]() {
		auto selected = tableOffers->selectedItems();
		auto selected2 = selected.at(0);
		string ceva = selected2->text().toStdString();
		Offer o = srv.searchOffer(ceva);
		editDestination->setText(QString::fromStdString(o.getDestination()));
		editPrice->setText(QString::number(o.getPrice()));
		editType->setText(QString::fromStdString(o.getType()));
		editName->setText(QString::fromStdString(o.getName()));
		});
}

void GUI::guiShowWishlistWindow() {
	wishlistWindow.show();
}

void GUI::reloadOfferList(vector<Offer> offers) {
	this->tableOffers->clearContents();
	this->tableOffers->setRowCount(offers.size());
	int lineNumber = 0;
	for (auto& offer : offers) {
		this->tableOffers->setItem(lineNumber, 0, new QTableWidgetItem(QString::fromStdString(offer.getName())));
		this->tableOffers->setItem(lineNumber, 1, new QTableWidgetItem(QString::fromStdString(offer.getDestination())));
		this->tableOffers->setItem(lineNumber, 2, new QTableWidgetItem(QString::fromStdString(offer.getType())));
		this->tableOffers->setItem(lineNumber, 3, new QTableWidgetItem(QString::number(offer.getPrice())));
		lineNumber++;
	}

}

void GUI::guiAddOffer() {
	try {
		//preluare date din QLineEdit-uri
		string name = editName->text().toStdString();
		string destination = editDestination->text().toStdString();
		string type = editType->text().toStdString();
		int price = editPrice->text().toInt();
		//golire dupa
		editName->clear();
		editDestination->clear();
		editType->clear();
		editPrice->clear();

		this->srv.addOffer(name, destination, type, price);
		addReportTypeButtons();
		this->reloadOfferList(this->srv.getAllOffers());

		//mesaj ca s-a adaugat
		QMessageBox::information(this, "Info", QString::fromStdString("Oferta adaugata cu succes."));
	}
	catch (OfferRepoException& re) {
		QMessageBox::information(this, "Warning", QString::fromStdString(re.getErrorMessage()));
	}
	catch (ValidationException& ve) {
		QMessageBox::information(this, "Warning", QString::fromStdString(ve.getErrorMessages()));
	}
}
void GUI::guiRemoveOffer() {
	try {
		//preluare date din QLineEdit
		string nameDelete = editNameDelete->text().toStdString();
		editNameDelete->clear();

		srv.removeOffer(nameDelete);
		addReportTypeButtons();
		reloadOfferList(srv.getAllOffers());
		QMessageBox::information(this, "Info", QString::fromStdString("Oferta stearsa!"));
	}
	catch (OfferRepoException& re) {
		QMessageBox::information(this, "Warning", QString::fromStdString(re.getErrorMessage()));
	}
	catch (ValidationException& ve) {
		QMessageBox::information(this, "Warning", QString::fromStdString(ve.getErrorMessages()));
	}
}
void GUI::guiUpdateOffer() {
	try {
		//preluare date din QLineEdit-uri
		string name = editName->text().toStdString();
		string destination = editDestination->text().toStdString();
		string type = editType->text().toStdString();
		int price = editPrice->text().toInt();
		//golire dupa
		editName->clear();
		editDestination->clear();
		editType->clear();
		editPrice->clear();
		srv.updateOffer(name, destination, type, price);
		addReportTypeButtons();
		reloadOfferList(srv.getAllOffers());

		QMessageBox::information(this, "Info", QString::fromStdString("Oferta modificata!"));
	}
	catch (OfferRepoException& re) {
		QMessageBox::information(this, "Warning", QString::fromStdString(re.getErrorMessage()));
	}
	catch (ValidationException& ve) {
		QMessageBox::information(this, "Warning", QString::fromStdString(ve.getErrorMessages()));
	}
}
void GUI::guiUndo() {
	try {
		srv.undo();
		addReportTypeButtons();
		reloadOfferList(srv.getAllOffers());
	}
	catch (OfferRepoException& re) {
		QMessageBox::information(this, "Warning", QString::fromStdString(re.getErrorMessage()));
	}
}

void GUI::addReportTypeButtons() {
	auto report{ srv.reportDestination() };
	for (auto it = buttons.begin(); it < buttons.end(); it++) {
		QPushButton* button = *it;
		if (report.find((*it)->text().toStdString()) == report.end()) {
			buttons.erase(it);
			delete button;
			break;
		}
	}
	for (const auto& entry : report) {
		string destination{ entry.first };
		int count{ entry.second };
		bool found{ false };
		for (auto button : buttons) {
			if (destination == button->text().toStdString()) {
				found = true;
				break;
			}
		}
		if (!found) {
			QPushButton* button = new QPushButton(QString::fromStdString(destination));
			buttons.push_back(button);
			layoutButtonDynamic->addWidget(button);
			QObject::connect(button, &QPushButton::clicked, this, [=]() {
				for (auto pair : srv.reportDestination()) {
					if (pair.first == button->text().toStdString()) {
						QMessageBox::information(this, "Count", QString::number(pair.second));
					}
				}
				});
		}
	}
}