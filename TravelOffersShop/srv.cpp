#include <assert.h>
#include <iostream>
#include "srv.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <map>

using std::copy_if;
using std::copy;
using std::back_inserter;
using std::shuffle;
using std::map;

void OfferStore::addOffer(string name, string destination, string type, int price) {
	Offer offer{ name, destination, type, price };
	val.valideaza(offer);
	repo.store(offer);
	undoActions.push_back(std::make_unique<UndoAdauga>(repo, offer));
}
void OfferStore::updateOffer(string searched_name, string new_destination, string new_type, int new_price) {
	Offer newOffer{ searched_name, new_destination, new_type, new_price };
	if (repo.searchOfferName(searched_name) == -1)
		throw OfferRepoException("Nume inexistent!\n");
	val.valideaza(newOffer);
	Offer oldOffer = repo.update(newOffer);
	undoActions.push_back(std::make_unique<UndoUpdate>(repo, oldOffer));
}
void OfferStore::removeOffer(string searched_name) {
	if (searched_name.length() < 2) {
		vector<string> errorMsg;
		errorMsg.push_back("Nume introdus incorect!");
		throw ValidationException(errorMsg);
	}
	int poz = repo.searchOfferName(searched_name);
	if (poz == -1) {
		throw OfferRepoException("Nume inexistent!\n");
	}
	Offer offer = repo.remove(searched_name);
	undoActions.push_back(std::make_unique<UndoSterge>(repo, offer));
}
Offer OfferStore::searchOffer(string searched_name) {
	if (searched_name.length() < 2) {
		vector<string> errorMsg;
		errorMsg.push_back("Nume introdus incorect!");
		throw ValidationException(errorMsg);
	}

	int poz = repo.searchOfferName(searched_name);
	if (poz == -1) {
		throw OfferRepoException("Oferta inexistenta!\n");
	}
	return repo.getOffer(poz);
}

vector<Offer> OfferStore::filterDestination(string filtering_destination) {
	if (filtering_destination.length() < 2) {
		vector<string> errorMsg;
		errorMsg.push_back("Destinatie introdusa incorect!");
		throw ValidationException(errorMsg);
	}
	vector<Offer> rez, offers = repo.getAllOffers();
	copy_if(offers.begin(), offers.end(), back_inserter(rez),
		[filtering_destination](Offer offer) {return offer.getDestination() == filtering_destination; });
	return rez;
}

map < string, int > OfferStore::reportDestination() {
	map < string, int > Destinations;
	for (const auto& offer : repo.getAllOffers()) {
		string curr_destination = offer.getDestination();
		Destinations[curr_destination]++;
	}
	return Destinations;
}

bool cmpAscendingDestination(Offer of1, Offer of2) {
	return of1.getDestination() < of2.getDestination();
}
bool cmpDescendingDestination(Offer of1, Offer of2) {
	return of1.getDestination() > of2.getDestination();
}
vector<Offer> OfferStore::sortDestination(int how) {
	vector<Offer> rez = repo.getAllOffers();
	if (how == 1)
		std::sort(rez.begin(), rez.end(), cmpAscendingDestination);
	else
		std::sort(rez.begin(), rez.end(), cmpDescendingDestination);
	return rez;
}

void OfferStore::undo() {
	if (undoActions.empty()) {
		throw OfferRepoException("Nu mai exista operatii");
	}
	undoActions.back()->doUndo();
	undoActions.pop_back();
}

void OfferStore::addToWishlist(string searched_name) {
	if (searched_name.length() < 2) {
		vector<string> errorMsg;
		errorMsg.push_back("Nume introdus incorect!");
		throw ValidationException(errorMsg);
	}
	int found = repo.searchOfferName(searched_name);
	if (found == -1) {
		throw OfferRepoException("Oferta inexistenta!\n");
	}
	for (auto offer : wishlist.getWishlist()) {
		if (offer.getName() == searched_name)
			throw OfferRepoException("Oferta deja existenta in wishlist!\n");
	}
	Offer offer(repo.getOffer(found));
	wishlist.store(offer);
}
void OfferStore::emptyWishlist() {
	if (wishlist.size() == 0) {
		throw OfferRepoException("Wishlist already empty!\n");
	}
	wishlist.EmptyWishlist();
}
void OfferStore::generateWishlist(int how_many) {
	if (unsigned(how_many) > repo.getAllOffers().size()) {
		throw OfferRepoException("Not enough existing offers!\n");
	}
	vector<Offer> offers = repo.getAllOffers();

	unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(offers.begin(), offers.end(), std::default_random_engine(seed));

	vector<Offer> wishlist_toAdd;
	wishlist.EmptyWishlist();
	copy(offers.begin(), offers.begin() + how_many, back_inserter(wishlist_toAdd));
	for (auto offer : wishlist_toAdd) {
		wishlist.store(offer);
	}
}

void testAddService() {
	OfferRepo testRepo;
	OfferValidator testVal;
	OfferWishlist testWishlist;
	OfferStore testService(testRepo, testVal, testWishlist);

	testService.addOffer("name1", "destination1", "type1", 1);
	testService.addOffer("name2", "destination2", "type2", 3);
	testService.addOffer("name3", "destination3", "type3", 3);

	assert(testService.getAllOffers().size() == 3);

	try {
		testService.addOffer("name1", "destination1", "type1", 1);
	}
	catch (OfferRepoException&) {
		assert(true);
	}

	try {
		testService.addOffer("", "destination4", "type4", 4);
	}
	catch (ValidationException& ve) {
		assert(ve.getErrorMessages() == "Numele trebuie sa aiba cel putin 2 caractere.\n");
	}

	try {
		testService.addOffer("name4", "", "type4", 4);
	}
	catch (ValidationException& ve) {
		assert(ve.getErrorMessages() == "Destinatia trebuie sa aiba cel putin 2 caractere.\n");
	}

	try {
		testService.addOffer("name4", "destination4", "", 4);
	}
	catch (ValidationException& ve) {
		assert(ve.getErrorMessages() == "Tipul trebuie sa aiba cel putin 1 caracter.\n");
	}

	try {
		testService.addOffer("name4", "destination4", "type4", -4);
	}
	catch (ValidationException& ve) {
		assert(ve.getErrorMessages() == "Pretul trebuie sa fie pozitiv.\n");
	}
}
void testUpdateService() {
	OfferRepo testRepo;
	OfferValidator testVal;
	OfferWishlist testWishlist;
	OfferStore testService(testRepo, testVal, testWishlist);

	testService.addOffer("name1", "destination1", "type1", 1);
	testService.addOffer("name2", "destination2", "type2", 3);
	testService.addOffer("name3", "destination3", "type3", 3);

	try {
		testService.updateOffer("name4", "destination4", "type4", 4);
	}
	catch (OfferRepoException& e) {
		assert(e.getErrorMessage() == "Nume inexistent!\n");
	}


	Offer compare_offer = testRepo.getOffer(2);
	assert(compare_offer.getDestination() == "destination3");

	testService.updateOffer("name3", "destination4", "type4", 4);

	compare_offer = testRepo.getOffer(2);
	assert(compare_offer.getDestination() == "destination4");
}
void testRemoveService() {
	OfferRepo testRepo;
	OfferValidator testVal;
	OfferWishlist testWishlist;
	OfferStore testService(testRepo, testVal, testWishlist);

	testService.addOffer("name1", "destination1", "type1", 1);
	testService.addOffer("name2", "destination2", "type2", 3);
	testService.addOffer("name3", "destination3", "type3", 3);

	assert(testService.getAllOffers().size() == 3);

	testService.removeOffer("name3");
	assert(testService.getAllOffers().size() == 2);

	try {
		testService.removeOffer("name4");
	}
	catch (OfferRepoException& e) {
		assert(e.getErrorMessage() == "Nume inexistent!\n");
	}

	try {
		testService.removeOffer("");
	}
	catch (ValidationException& ve) {
		assert(ve.getErrorMessages() == "Nume introdus incorect!\n");
	}
}
void testSearchService() {
	OfferRepo testRepo;
	OfferValidator testVal;
	OfferWishlist testWishlist;
	OfferStore testService(testRepo, testVal, testWishlist);

	testService.addOffer("name1", "destination1", "type1", 1);
	testService.addOffer("name2", "destination2", "type2", 3);
	testService.addOffer("name3", "destination3", "type3", 3);

	Offer searched_offer{ "name3", "destination3", "type3", 3 };
	Offer found_offer = testService.searchOffer("name3");
	assert(found_offer.getDestination() == searched_offer.getDestination());

	try {
		testService.searchOffer("name4");
	}
	catch (OfferRepoException& re) {
		assert(re.getErrorMessage() == "Oferta inexistenta!\n");
	}

	try {
		testService.searchOffer("");
	}
	catch (ValidationException& ve) {
		assert(ve.getErrorMessages() == "Nume introdus incorect!\n");
	}
}
void testFiltrareService() {
	OfferRepo testRepo;
	OfferValidator testVal;
	OfferWishlist testWishlist;
	OfferStore testService(testRepo, testVal, testWishlist);

	testService.addOffer("name1", "destination1", "type1", 1);
	testService.addOffer("name2", "destination2", "type2", 3);
	testService.addOffer("name3", "destination3", "type3", 3);
	testService.addOffer("name4", "destination3", "type4", 4);

	vector<Offer> v1 = testService.filterDestination("destination3");
	assert(v1.size() == 2);

	vector<Offer> v_empty = testService.filterDestination("Deeeee");
	assert(v_empty.size() == 0);

	try {
		vector<Offer> v_error = testService.filterDestination("");
	}
	catch (ValidationException& ve) {
		assert(ve.getErrorMessages() == "Destinatie introdusa incorect!\n");
	}
}
void testSortService() {
	OfferRepo testRepo;
	OfferValidator testVal;
	OfferWishlist testWishlist;
	OfferStore testService(testRepo, testVal, testWishlist);

	testService.addOffer("name4", "destination4", "type4", 4);
	testService.addOffer("name1", "destination1", "type1", 1);
	testService.addOffer("name2", "destination2", "type2", 3);
	testService.addOffer("name3", "destination3", "type3", 3);

	Offer offer1 = testRepo.getOffer(0);
	assert(offer1.getDestination() == "destination4");

	vector<Offer> testRez = testService.getAllOffers();
	assert(testRez[0].getDestination() == "destination4");

	testRez = testService.sortDestination(1);
	assert(testRez[0].getDestination() == "destination1");

	testRez = testService.sortDestination(2);
	assert(testRez[0].getDestination() == "destination4");
}
void testUndoService() {
	OfferRepo testRepo;
	OfferValidator testVal;
	OfferWishlist testWishlist;
	OfferStore testService(testRepo, testVal, testWishlist);
	testService.addOffer("name1", "destination1", "type1", 1);
	testService.addOffer("name2", "destination2", "type2", 3);
	testService.addOffer("name3", "destination3", "type3", 3);
	assert(testService.getAllOffers().size() == 3);
	testService.undo();
	testService.undo();
	assert(testService.getAllOffers().size() == 1);
	testService.removeOffer("name1");
	assert(testService.getAllOffers().size() == 0);
	testService.undo();
	assert(testService.getAllOffers().size() == 1);
	testService.updateOffer("name1", "destination2", "type2", 2);
	vector<Offer> offers = testService.getAllOffers();
	assert(offers[0].getDestination() == "destination2");
	testService.undo();
	offers = testService.getAllOffers();
	assert(offers[0].getDestination() == "destination1");
}

void testStoreEmptyWishlist() {
	OfferRepo testRepo;
	OfferValidator testVal;
	OfferWishlist testWishlist;
	OfferStore testService(testRepo, testVal, testWishlist);
	testService.addOffer("name1", "destination1", "type1", 1);
	testService.addOffer("name2", "destination2", "type2", 3);
	testService.addOffer("name3", "destination3", "type3", 3);
	testService.addToWishlist("name2");
	assert(testWishlist.size() == 1);
	try {
		testService.addToWishlist("1");
	}
	catch (ValidationException&) {
		assert(true);
	}
	try {
		testService.addToWishlist("nonexistent_name");
	}
	catch (OfferRepoException& re) {
		assert(re.getErrorMessage() == "Oferta inexistenta!\n");
	}

	testService.addToWishlist("name1");
	assert(testWishlist.size() == 2);
	testService.emptyWishlist();
	assert(testWishlist.size() == 0);

	try {
		testService.emptyWishlist();
	}
	catch (OfferRepoException& re) {
		assert(re.getErrorMessage() == "Wishlist already empty!\n");
	}

}
void testGenerateWishlist() {
	OfferRepo testRepo;
	OfferValidator testVal;
	OfferWishlist testWishlist;
	OfferStore testService(testRepo, testVal, testWishlist);
	testService.addOffer("name1", "destination1", "type1", 1);
	testService.addOffer("name2", "destination2", "type2", 3);
	testService.addOffer("name3", "destination3", "type3", 3);
	try {
		testService.generateWishlist(4);
	}
	catch (OfferRepoException&) {
		assert(true);
	}
	assert(testWishlist.size() == 0);
	testService.generateWishlist(2);
	assert(testWishlist.size() == 2);
}

void testsService() {
	testAddService();
	testUpdateService();
	testRemoveService();
	testSearchService();
	testFiltrareService();
	testSortService();
	testUndoService();

	testStoreEmptyWishlist();
	testGenerateWishlist();
}