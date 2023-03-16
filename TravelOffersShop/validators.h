#pragma once

#include "Offer.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

class ValidationException {
	vector<string> errorMsg;
public:
	ValidationException(vector<string> errorMessages) : errorMsg{ errorMessages } {};

	string getErrorMessages() {
		string fullMsg = "";
		for (const string e : errorMsg) {
			fullMsg += e + "\n";
		}
		return fullMsg;
	}
};

/*
Clasa pentru validarea ofertelor
O oferta este valida daca:
	are numele format din mai mult de 2 caractere
	are destinatie format din mai mult de 2 caractere
	are tip format din sir de caractere nevid
	pretul ofertei pozitiv
*/
class OfferValidator {
public:
	void valideaza(const Offer& offer) {
		vector<string> errors;
		if (offer.getName().length() < 2)
			errors.push_back("Numele trebuie sa aiba cel putin 2 caractere.");
		if (offer.getDestination().length() < 2)
			errors.push_back("Destinatia trebuie sa aiba cel putin 2 caractere.");
		if (offer.getType().length() == 0)
			errors.push_back("Tipul trebuie sa aiba cel putin 1 caracter.");
		if (offer.getPrice() < 0)
			errors.push_back("Pretul trebuie sa fie pozitiv.");

		if (errors.size() > 0)
			throw ValidationException(errors);
	}
};