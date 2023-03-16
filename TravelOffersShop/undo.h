#pragma once

#include "Offer.h"
#include "OfferRepo.h"

class ActiuneUndo {
public:
	virtual void doUndo() = 0;
	virtual ~ActiuneUndo() = default;
};

class UndoAdauga : public ActiuneUndo {
	Offer addedOffer;
	OfferRepo& repo;
public:
	UndoAdauga(OfferRepo& repo, const Offer& offer) :
		repo{ repo },
		addedOffer{ offer } {}
	void doUndo() override {
		repo.remove(addedOffer.getName());
	}
};

class UndoSterge : public ActiuneUndo {
	Offer deletedOffer;
	OfferRepo& repo;
public:
	UndoSterge(OfferRepo& repo, const Offer& offer) :
		repo{ repo },
		deletedOffer{ offer } {}
	void doUndo() override {
		repo.store(deletedOffer);
	}
};

class UndoUpdate : public ActiuneUndo {
	Offer oldOffer;
	OfferRepo& repo;
public:
	UndoUpdate(OfferRepo& repo, const Offer& offer) :
		repo{ repo },
		oldOffer{ offer } {}
	void doUndo() override {
		repo.update(oldOffer);
	}
};