#include "card.h"
#include "deck.h"

using namespace std;

Deck::~Deck() {}

vector<shared_ptr<Card>> &Deck::getDeck() {return theDeck;}

bool Deck::isEmpty() {return theDeck.size() == 0;}

void Deck::push(shared_ptr<Card> c) {theDeck.emplace_back(c);}

void Deck::popfirst() {
    if (isEmpty()) throw 4; // Number; The deck is empty
    theDeck.erase(theDeck.begin());
}

void Deck::shuffle(int i) {
    srand(time(NULL)+i);
    random_shuffle (theDeck.begin(), theDeck.end());
} 




    
