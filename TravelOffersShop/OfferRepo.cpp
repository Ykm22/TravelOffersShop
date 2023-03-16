#include "OfferRepo.h"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <fstream>

using std::find;
using std::find_if;
using std::distance;

void OfferRepo::loadFromFile() {
	std::ifstream in(file_name);
	string name;
	in >> name;
	string destination;
	in >> destination;
	string type;
	in >> type;
	int price;
	in >> price;
	Offer offer{ name, destination, type, price };
	OfferRepo::store(offer);
	in.close();
}

const vector<Offer> OfferRepo::getAllOffers() {
	vector<Offer> rez;
	for (auto offer : this->allOffers) {
		rez.push_back(offer);
	}
	return rez;
}

bool OfferRepo::exists(const Offer& offer) {
	try {
		search(offer.getName(), offer.getDestination(), offer.getType(), offer.getPrice());
		return true;
	}
	catch (OfferRepoException&) {
		return false;
	}
}
Offer OfferRepo::search(string name, string destination, string type, int price) {
	Offer searched_offer{ name, destination, type, price };
	auto is_even = [searched_offer](Offer offer) {return offer == searched_offer; };
	auto found = find_if(allOffers.begin(), allOffers.end(), is_even);
	if (found != allOffers.end()) {
		return searched_offer;
	}
	throw OfferRepoException("Nu exista oferta aceasta!\n");
}
int OfferRepo::searchOfferName(string searched_name) {
	auto is_even = [searched_name](Offer offer) {return offer.getName() == searched_name; };
	auto poz = find_if(allOffers.begin(), allOffers.end(), is_even);
	if (poz != allOffers.end())
		return distance(allOffers.begin(), poz);
	return -1;
}

void OfferRepo::store(const Offer& offer) {
	if (exists(offer)) {
		throw OfferRepoException("Ofersta existenta!\n");
	}
	this->allOffers.push_back(offer);
}

Offer OfferRepo::remove(const string& searched_name) {
	int poz = searchOfferName(searched_name);
	if (poz == -1) {
		throw OfferRepoException("Oferta inexistenta!\n");
	}
	Offer offer = getOffer(poz);
	allOffers.erase(allOffers.begin() + poz);
	return offer;
}
Offer OfferRepo::update(const Offer& offer) {
	auto is_even = [offer](Offer searching_offer) {return searching_offer.getName() == offer.getName(); };
	auto found = find_if(allOffers.begin(), allOffers.end(), is_even);
	if (found == allOffers.end()) {
		throw OfferRepoException("Nume inexistent!\n");
	}
	int poz2 = distance(allOffers.begin(), found);
	Offer oldOffer = allOffers[poz2];
	allOffers[poz2] = offer;
	return oldOffer;
}

void testAddRepo() {
	OfferRepo testRepo;
	Offer offer1{ "name1", "destination1", "type1", 1 };
	testRepo.store(offer1);
	assert(testRepo.getAllOffers().size() == 1);


	Offer offer2{ "name2", "destination2", "type2", 2 };
	Offer offer3{ "name1", "destination1", "type1", 1 };

	try {
		testRepo.store(offer3);
	}
	catch (OfferRepoException) {
		assert(true);
	}
}
void testFindRepo() {
	OfferRepo testRepo;
	Offer offer1{ "name1", "destination1", "type1", 1 };
	Offer offer2{ "name2", "destination2", "type2", 2 };
	Offer offer3{ "name3", "destination3", "type3", 3 };
	testRepo.store(offer1);
	testRepo.store(offer2);
	assert(testRepo.exists(offer1));
	assert(!testRepo.exists(offer3));

	auto foundOffer = testRepo.search("name1", "destination1", "type1", 1);
	assert(foundOffer.getDestination() == "destination1");
	assert(foundOffer.getName() == "name1");
	assert(foundOffer.getPrice() == 1);
	assert(foundOffer.getType() == "type1");

	try {
		testRepo.search("name4", "destination4", "type4", 4);
	}
	catch (OfferRepoException& e) {
		assert(e.getErrorMessage() == "Nu exista oferta aceasta!\n");
	}
}
void testRemoveRepo() {
	OfferRepo testRepo;
	Offer offer1{ "name1", "destination1", "type1", 1 };
	Offer offer2{ "name2", "destination2", "type2", 2 };
	Offer offer3{ "name3", "destination3", "type3", 3 };
	testRepo.store(offer1);
	testRepo.store(offer2);
	assert(testRepo.getAllOffers().size() == 2);

	testRepo.remove("name1");
	assert(testRepo.getAllOffers().size() == 1);

	try {
		testRepo.remove("name3");
	}
	catch (OfferRepoException& e) {
		assert(e.getErrorMessage() == "Oferta inexistenta!\n");
	}
}
void testUpdateRepo() {
	OfferRepo testRepo;
	Offer offer1{ "name1", "destination1", "type1", 1 };
	Offer offer2{ "name2", "destination2", "type2", 2 };
	Offer offer3{ "name3", "destination3", "type3", 3 };
	testRepo.store(offer1);
	testRepo.store(offer2);

	try {
		testRepo.update(offer3);
	}
	catch (OfferRepoException& e) {
		assert(e.getErrorMessage() == "Nume inexistent!\n");
	}

	Offer old_offer = testRepo.getOffer(1);
	assert(old_offer.getDestination() == "destination2");

	offer3.setName("name2");
	testRepo.update(offer3);

	Offer new_offer = testRepo.getOffer(1);
	assert(new_offer.getDestination() == "destination3");
}

void testsRepo() {
	testAddRepo();
	testFindRepo();
	testRemoveRepo();
	testUpdateRepo();
}