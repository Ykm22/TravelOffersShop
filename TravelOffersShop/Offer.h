#pragma once
#include <string>
#include <iostream>

using std::string;

class Offer {
private:
	string name;
	string destination;
	string type;
	int price;

public:
	Offer() = default;
	Offer(const string& _name, const string& _destination, const string& _type, int _price) :
		name{ _name },
		destination{ _destination },
		type{ _type },
		price{ _price } {
	}

	Offer(const Offer& other) :
		name{ other.name },
		destination{ other.destination },
		type{ other.type },
		price{ other.price } {
	}

	const string& getName() const;
	const string& getDestination() const;
	const string& getType() const;
	int getPrice() const;

	void setName(const string& new_name);
	void setDestination(const string& new_destination);
	void setType(const string& new_type);
	void setPrice(int new_price);

	bool operator==(const Offer& other);
};

void testsDomain();