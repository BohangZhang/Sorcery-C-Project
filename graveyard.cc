#include "minion.h"
#include "graveyard.h"
#include "player.h"
#include "database.h"

using namespace std;

Graveyard::~Graveyard() {}
    
vector<shared_ptr<Minion>> Graveyard::getGrave() {return theGrave;}

shared_ptr<Minion> Graveyard::top() {return theGrave[theGrave.size() - 1];}

bool Graveyard::isEmpty() {return theGrave.size() == 0;}

void Graveyard::pushfirst(shared_ptr<Minion> m) {
    if (m->getType() == "Enchantment") {
        shared_ptr<Minion> m1 = m->getMaster()->getDatabase()->getMinion(m->getOriginName())->cloneMinion();
        shared_ptr<Card> c = m1;
        c->setMaster(m->getMaster());
        theGrave.emplace_back(m1);
    }
    else {
        shared_ptr<Minion> m1 = m->getMaster()->getDatabase()->getMinion(m->getName())->cloneMinion();
        shared_ptr<Card> c = m1;
        c->setMaster(m->getMaster());
        theGrave.emplace_back(m1);
    }
}

void Graveyard::popfirst() {
    if (!isEmpty()) {
        theGrave.erase(theGrave.begin());
    }
}
