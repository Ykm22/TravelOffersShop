#include "Offer.h"
#include <vector>
#include "Observer.h"

using std::vector;

class OfferWishlistException {
private:
	string errorMsg;
public:
	OfferWishlistException(string msg) :errorMsg{ msg } {}
	string getErrorMessage() {
		return errorMsg;
	}
};

class OfferWishlist : public Observable {
private:
	vector<Offer> wishlist;
public:
	OfferWishlist() = default;
	void store(Offer offer);

	int size();

	void EmptyWishlist();

	vector<Offer> getWishlist();

};