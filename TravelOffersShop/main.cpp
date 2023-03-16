#include "TravelOffersShop.h"
#include <QtWidgets/QApplication>
#include "GUI.h"

void testAll() {
    testsDomain();
    std::cout << "Finished domain tests\n";
    testsRepo();
    std::cout << "Finished repository tests\n";
    testsService();
    std::cout << "Finished service tests\n";
}

int main(int argc, char* argv[])
{
    //-testAll();
    QApplication a(argc, argv);
    OfferRepo repo{ "offers.txt" };
    assert(repo.getAllOffers().size() == 1);
    OfferValidator val;
    OfferWishlist wishlist;
    OfferStore srv{ repo, val, wishlist };
    srv.addOffer("name4", "destination1", "type4", 4);
    srv.addOffer("name1", "destination1", "type1", 1);
    srv.addOffer("name2", "destination2", "type2", 3);
    srv.addOffer("name3", "destination3", "type3", 3);
    GUI gui{ srv };
    gui.show();
    return a.exec();
}
