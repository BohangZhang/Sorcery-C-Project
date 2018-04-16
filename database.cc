#include "card.h"
#include "minion.h"
#include "database.h"

using namespace std;

Database::~Database() {}

shared_ptr<Card> Database::getCard(string s) {
    return allcards[s];
}

shared_ptr<Minion> Database::getMinion(string s) {
    return allminions[s];
}

void Database::push(shared_ptr<Card> card) {
    allcards[card->getName()] = card;
}

void Database::pushMinion(shared_ptr<Minion> minion) {
    allminions[minion->getName()] = minion;
}
