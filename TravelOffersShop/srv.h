#pragma once
#include "OfferRepo.h"
#include "OfferWishlist.h"
#include "validators.h"
#include <iostream>
#include <assert.h>
#include <map>
#include "undo.h"
#include <memory>
#include <vector>

using std::map;
using std::unique_ptr;

class OfferStore {
private:
	OfferRepo& repo;
	OfferValidator& val;
	OfferWishlist& wishlist;
	std::vector<unique_ptr<ActiuneUndo>> undoActions;
public:
	OfferStore(OfferRepo& offerRepo, OfferValidator& val, OfferWishlist& wishlist) :
		repo{ offerRepo },
		val{ val },
		wishlist{ wishlist } {}

	//constructor de copiere
	//punem delete fiindca nu vrem sa se faca nicio copie la Service
	//(in aplicatie avem 1 singur Service)
	OfferStore(const OfferStore& other) = delete;

	const vector<Offer> getAllOffers() {
		return this->repo.getAllOffers();
	}

	OfferWishlist& getWishlist() {
		return wishlist;
	}

	/*
	Adauga o oferta
	*@param name: numele ofertei care se adauga (string)
	*@param destination: destinatia ofertei care se adauga (string)
	*@param type: tipul ofertei care se adauga (string)
	*@durata price: pretul ofertei care se adauga (int)
	*
	*@throws
	*	RepoException daca mai exista oferta deja
	*	ValidationException daca oferta nu este valida
	*/
	void addOffer(string name, string destination, string type, int price);

	/*
	Modifica o oferta cu acelasi numa
	@param searched_name: numele ofertei pe care vrem sa o modificam
	@param new_destination: noua destinatie
	@param new_type: noul tip
	@param new_price: noul pret

	@throws:
		RepoException daca nu exista numele ofertei in lista
		ValidationException daca oferta nu este valida
	*/
	void updateOffer(string searched_name, string new_destination, string new_type, int new_price);

	/*
	Sterge o oferta dupa un nume cautat
	@param searched_name: numele ofertei pe care vrem sa o stergem

	@throws:
		RepoException daca nu exista numele in lista
		ValidationException daca datele sunt introduse incorect
	*/
	void removeOffer(string searched_name);

	/*
	Cauta o oferta dupa nume
	@param searched_name: numele ofertei pe care o cautam

	@return Oferta daca exista
	@throws:
		RepoException daca oferta nu exista
		ValidationException daca datele sunt introduse incorect
	*/
	Offer searchOffer(string searched_name);

	/*
	Filtrarea dupa o anumita destinatie

	@param filtering_destination: destinatia dupa care se va filtra

	@return lista in urma filtrarii
	@throws:
		ValidationException daca datele sunt introduse incorect
	*/
	vector<Offer> filterDestination(string filtering_destination);

	/*
	Sortarea dupa o anumita destinatie

	@param how: daca sortam crescator sau descrescator

	@return lista in urma sortarii
	@throws
		ValidationException daca datele sunt introduse incorect
	*/
	vector<Offer> sortDestination(int how);

	/*
	* Operatia de undo
	*/
	void undo();

	/*
	Adaugarea unei oferte in wishlist
	@param searched_name: numele ofertei pe care vrem sa o adaugam in wishlist
	@throws
		ValidationException daca datele sunt introduse incorect
	*/
	void addToWishlist(string searched_name);

	//golirea wishlist-ului
	void emptyWishlist();

	/*
	Generarea random a unui wishlist
	@param how_many: cate oferte se vor afla in wishlist
	*/
	void generateWishlist(int how_many);

	/*
	Afisarea numarului de aparitii a destinatiei
		date ca parametru
	@param destination: destinatia a carei frecventa o cautam
	*/
	map < string, int > reportDestination();

};

void testsService();