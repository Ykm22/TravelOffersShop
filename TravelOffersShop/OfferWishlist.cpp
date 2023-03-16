#include "OfferWishlist.h"

void OfferWishlist::store(Offer offer) {
	wishlist.push_back(offer);
	notify();
}

int OfferWishlist::size() {
	return wishlist.size();
}

void OfferWishlist::EmptyWishlist() {
	wishlist.clear();
	notify();
}

vector<Offer> OfferWishlist::getWishlist() {
	vector<Offer> rez;
	for (auto offer : wishlist)
		rez.push_back(offer);
	return rez;
}