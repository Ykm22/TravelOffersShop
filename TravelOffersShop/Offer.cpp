#include "Offer.h"
#include <assert.h>

const string& Offer::getName() const {
	return this->name;
}

const string& Offer::getDestination() const {
	return this->destination;
}

const string& Offer::getType() const {
	return this->type;
}

int Offer::getPrice() const {
	return this->price;
}

void Offer::setName(const string& new_name) {
	this->name = new_name;
}

void Offer::setDestination(const string& new_destination) {
	this->destination = new_destination;
}

void Offer::setType(const string& new_type) {
	this->type = new_type;
}

void Offer::setPrice(int new_price) {
	this->price = new_price;
}

bool Offer::operator==(const Offer& other) {
	if (this->getDestination() != other.getDestination())
		return false;
	if (this->getName() != other.getName())
		return false;
	if (this->getPrice() != other.getPrice())
		return false;
	if (this->getType() != other.getType())
		return false;
	return true;
}

void testsGetSet() {
	Offer o("name_test", "destination_test", "type_test", 100);
	assert(o.getName() == "name_test");
	assert(o.getDestination() == "destination_test");
	assert(o.getType() == "type_test");
	assert(o.getPrice() == 100);

	Offer o2(o);
	assert(o.getDestination() == o2.getDestination());
	assert(o.getName() == o2.getName());
	assert(o.getPrice() == o2.getPrice());
	assert(o.getType() == o2.getType());

	o.setName("new_name");
	o.setDestination("new_destination");
	o.setType("new_type");
	o.setPrice(200);

	assert(o.getDestination() != o2.getDestination());
	assert(o.getName() != o2.getName());
	assert(o.getPrice() != o2.getPrice());
	assert(o.getType() != o2.getType());
}

void testsDomain() {
	testsGetSet();
}