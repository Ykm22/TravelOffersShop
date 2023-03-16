#include "WishlistGUI.h"


void WishlistGUI::initGUIComponents() {
	layoutMain = new QHBoxLayout;
	this->setLayout(layoutMain);

	QWidget* leftSide = new QWidget;
	QVBoxLayout* layoutLeft = new QVBoxLayout;
	leftSide->setLayout(layoutLeft);

	//widget tip form pt adaugarea in wishlist
	QWidget* formAddWishlist = new QWidget;
	QFormLayout* layoutFormAddWishlist = new QFormLayout;
	formAddWishlist->setLayout(layoutFormAddWishlist);

	labelNameToAdd = new QLabel("Nume: ");
	editAddWishlist = new QLineEdit;
	layoutFormAddWishlist->addRow(labelNameToAdd, editAddWishlist);
	buttonAddWishlist = new QPushButton("Adauga in wishlist");
	layoutFormAddWishlist->addWidget(buttonAddWishlist);

	layoutLeft->addWidget(formAddWishlist);


	//widget tip form pt selectarea unei linii din lista
	QWidget* formFocusWishlist = new QWidget;
	QFormLayout* layoutFormFocusWishlist = new QFormLayout;
	formFocusWishlist->setLayout(layoutFormFocusWishlist);

	labelDestination = new QLabel("Destinatie: ");
	labelType = new QLabel("Tip: ");
	destinationText = new QLineEdit;
	typeText = new QLineEdit;

	layoutFormFocusWishlist->addRow(labelDestination, destinationText);
	layoutFormFocusWishlist->addRow(labelType, typeText);

	layoutLeft->addWidget(formFocusWishlist);


	//widget tip form pt generare random
	QWidget* formGenerateWishlist = new QWidget;
	QFormLayout* layoutFormGenerateWishlist = new QFormLayout;
	formGenerateWishlist->setLayout(layoutFormGenerateWishlist);

	labelGenerateWishlist = new QLabel("Nr oferte de generat: ");
	editGenerateWishlist = new QLineEdit;
	buttonGenerateWishlist = new QPushButton("Generare");

	layoutFormGenerateWishlist->addRow(labelGenerateWishlist, editGenerateWishlist);
	layoutFormGenerateWishlist->addWidget(buttonGenerateWishlist);

	layoutLeft->addWidget(formGenerateWishlist);


	//empty wishlist
	buttonEmptyWishlist = new QPushButton("Golire");
	layoutLeft->addWidget(buttonEmptyWishlist);

	//widget tip form pt export
	QWidget* formExportWishlist = new QWidget;
	QFormLayout* layoutFormExportWishlist = new QFormLayout;
	formExportWishlist->setLayout(layoutFormExportWishlist);

	labelExportWishlist = new QLabel("Fisier .csv de export: ");
	editExportWishlist = new QLineEdit;
	buttonExportWishlist = new QPushButton("Exportare");

	layoutFormExportWishlist->addRow(labelExportWishlist, editExportWishlist);
	layoutFormExportWishlist->addWidget(buttonExportWishlist);

	layoutLeft->addWidget(formExportWishlist);


	//butoane pentru celelalte ferestre
	buttonLabelWindow = new QPushButton("Label window");
	buttonLabelWindow->setStyleSheet("background-color: cyan");

	buttonDrawWindow = new QPushButton("Draw window");
	buttonDrawWindow->setStyleSheet("background-color: yellow");

	layoutLeft->addWidget(buttonLabelWindow);
	layoutLeft->addWidget(buttonDrawWindow);



	//close this window
	buttonClose = new QPushButton("Close");
	layoutLeft->addWidget(buttonClose);




	///right side
	QWidget* rightSide = new QWidget;
	QVBoxLayout* layoutRight = new QVBoxLayout;
	rightSide->setLayout(layoutRight);




	//cosul ca lista
	layoutRight->addWidget(listView);

	layoutMain->addWidget(leftSide);
	layoutMain->addWidget(rightSide);
}

void WishlistGUI::connectSignalSlots() {
	QObject::connect(buttonAddWishlist, &QPushButton::clicked, this, &WishlistGUI::AddToWishlist);
	QObject::connect(buttonEmptyWishlist, &QPushButton::clicked, this, &WishlistGUI::EmptyWishlist);
	QObject::connect(buttonGenerateWishlist, &QPushButton::clicked, this, &WishlistGUI::GenerateWishlist);
	QObject::connect(buttonExportWishlist, &QPushButton::clicked, this, &WishlistGUI::ExportWishlist);
	QObject::connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged, [&]() {
		if (listView->selectionModel()->selectedIndexes().isEmpty())
			return;
		auto selectionIndex = listView->selectionModel()->selectedIndexes().at(0);
		QString destination = selectionIndex.data(Qt::DisplayRole).toString();
		QString type = selectionIndex.data(Qt::UserRole).toString();
		destinationText->setText(destination);
		typeText->setText(type);
		});

	QObject::connect(buttonClose, &QPushButton::clicked, this, &WishlistGUI::close);
	QObject::connect(buttonLabelWindow, &QPushButton::clicked, this, &WishlistGUI::ShowLabelWindow);
	QObject::connect(buttonDrawWindow, &QPushButton::clicked, this, &WishlistGUI::ShowDrawWindow);
}
void WishlistGUI::AddToWishlist() {
	try {
		string name = editAddWishlist->text().toStdString();
		editAddWishlist->clear();
		srv.addToWishlist(name);
		reloadWishlist(srv.getWishlist().getWishlist());
		QMessageBox::information(this, "Info", QString::fromStdString("Oferta adaugata in cos!"));
	}
	catch (OfferRepoException& re) {
		QMessageBox::information(this, "Warning", QString::fromStdString(re.getErrorMessage()));
	}
	catch (ValidationException& ve) {
		QMessageBox::information(this, "Warning", QString::fromStdString(ve.getErrorMessages()));
	}
}
void WishlistGUI::EmptyWishlist() {
	try {
		srv.emptyWishlist();
		reloadWishlist(srv.getWishlist().getWishlist());
		QMessageBox::information(this, "Info", QString::fromStdString("Wishlist golit!"));
	}
	catch (OfferRepoException& re) {
		QMessageBox::information(this, "Warning", QString::fromStdString(re.getErrorMessage()));
	}
}
void WishlistGUI::GenerateWishlist() {
	try {
		int how_many = editGenerateWishlist->text().toInt();
		editGenerateWishlist->clear();
		srv.generateWishlist(how_many);
		reloadWishlist(srv.getWishlist().getWishlist());
		QMessageBox::information(this, "Info", QString::fromStdString("Wishlist generat random!"));
	}
	catch (OfferRepoException& re) {
		QMessageBox::information(this, "Warning", QString::fromStdString(re.getErrorMessage()));
	}
}
void WishlistGUI::ExportWishlist() {
	string file_name = editExportWishlist->text().toStdString();
	editExportWishlist->clear();
	if (srv.getWishlist().size() == 0) {
		QMessageBox::information(this, "Warning", QString::fromStdString("Nimic de exportat! Wishlist gol!"));
		return;
	}
	if (file_name.size() < 5 || file_name.find_last_of(".csv") == file_name.npos) {
		QMessageBox::information(this, "Warning", QString::fromStdString("Introduceti un fisier .csv!"));
		return;
	}
	std::ofstream fout(file_name);
	vector<Offer> to_print = srv.getWishlist().getWishlist();
	for (auto offer : to_print) {
		fout << "Nume: " << offer.getName() << " ";
		fout << "Destinatie: " << offer.getDestination() << " ";
		fout << "Type: " << offer.getType() << " ";
		fout << "Price: " << offer.getPrice() << "\n";
	}
	QMessageBox::information(this, "Info", QString::fromStdString("Wishlist adaugat in fisier!"));
	fout.close();

}
void WishlistGUI::ShowLabelWindow() {
	auto offerCounter = new OfferCounter(srv.getWishlist());
	offerCounter->show();
}
void WishlistGUI::ShowDrawWindow() {
	auto offerDraw = new OfferDraw(srv.getWishlist());
	offerDraw->show();
}
void WishlistGUI::reloadWishlist(vector<Offer> wishlist) {
	wishlistOffers->setOffers(wishlist);
}