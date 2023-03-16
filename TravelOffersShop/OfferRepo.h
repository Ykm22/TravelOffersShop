#pragma once
#include "Offer.h"

#include <vector>
#include <string>
#include <ostream>
#include <map>
#include "Observer.h"

typedef Offer TElem;

using std::vector;
using std::string;
using std::ostream;
using std::map;

class OfferRepoException {
private:
	string errorMsg;
public:
	OfferRepoException(string errorMsg) :errorMsg{ errorMsg } {}
	string getErrorMessage() {
		return this->errorMsg;
	}
};

class OfferRepo {
private:
	string file_name;
	vector<Offer> allOffers;
	void loadFromFile();

public:
	OfferRepo(string file_name) :file_name{ file_name } {
		loadFromFile();
	}
	//default constructor
	OfferRepo() {};

	//nu vrem sa facem copie la repo -> sa fie doar 1 repo in app
	OfferRepo(const OfferRepo& ot) = delete;

	/*
	Returneaza o lista cu toate ofertele
	@return: lista cu ofertele
	*/
	const vector<Offer> getAllOffers();

	/*
	Verifica daca o oferta data exista in lista

	@param offer: oferta care se cauta in lista
	@return: true daca oferta exista, false altfel
	*/
	bool exists(const Offer& offer);

	/*
	Cauta o oferta

	@param name: numele dupa care se cauta
	@param destination: destinatia dupa care se cauta
	@param type: tipul dupa care se cauta
	@param price: pretul dupa care se cauta
	@returns: entitatea Offer (daca aceasta exista)
	@throws: RepoException daca nu exista oferta*/
	Offer search(string name, string destination, string type, int price);


	/*
	Cautam numele unei oferte in repo

	@param searched_name: numele pe care il cautam
	@return pozitia la care o gasim, daca exista, -1 altfel
	*/
	int searchOfferName(string searched_name);

	/*
	Adaugam o oferta in lista
	@param offer: oferta care se adauga (Offer)
	@return -
	post: oferta va fi adaugata in lista
	@throws: RepoException daca exista deja acea oferta
	*/
	void store(const Offer& offer);

	/*
	Sterge o oferta din lista
	@param searched_name: numele ofertei care este eliminata din lista
	@return: deleted offer
	@throws: RepoException daca nu exista oferta in lista
	*/
	Offer remove(const string& searched_name);

	/*
	Modifica o oferta dupa nume

	@param offer: oferta noua care va inlocui in lista o oferta cu acelasi nume

	@throws: RepoException daca nu exista oferta cu numele name
	*/
	Offer update(const Offer& offer);

	/*
	Intoarce oferta de la o anumita pozitie din lista
	*/
	Offer getOffer(int poz) {
		//return allOffers.get(poz);
		return allOffers[poz];
	}

};

void testsRepo();