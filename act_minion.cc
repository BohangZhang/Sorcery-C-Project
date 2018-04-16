#include "act_minion.h"
#include "player.h"
#include "card.h"
#include "database.h"
#include "slot.h"
#include "game.h"
#include <sstream>

using namespace std;

Act_Minion::Act_Minion(string name, int cost, string effect, string type, int attack, int defense, int abilityCost) :
    Minion{name, cost, type, attack, defense, effect}, abilityCost{abilityCost} {}

Act_Minion::~Act_Minion() {}

int Act_Minion::getAbilityCost() {return this->abilityCost;}

void Act_Minion::use() {
    if (getSilent()) throw 1; // Number; The minion has been silented
    if (getMaster()->isSlotFull()) throw 2; // You have already own 5 Minions!
    if (name == "Apprentice Summoner") {
        shared_ptr<Minion> m = this->master->getDatabase()->getMinion("Air Elemental")->cloneMinion();
        shared_ptr<Card> p = m;
        p->setMaster(master);
        this->getMaster()->getSlot()->push(m);
        this->getMaster()->getGame()->notify("Enter", m);
    }
    if (name == "Master Summoner") {
        int i = this->master->getSlot()->getSize();
        int count = 0;
        while (!(getMaster()->isSlotFull()) && (count < 3)) {
            shared_ptr<Minion> m = this->master->getDatabase()->getMinion("Air Elemental")->cloneMinion();
            shared_ptr<Card> p = m;
            p->setMaster(master);
            this->getMaster()->getSlot()->push(m);
            this->getMaster()->getGame()->notify("Enter", m);
            ++count;
        }
    }
}


void Act_Minion::use(int p, string t) {
    if (getSilent()) throw 1;//Number; The minion has been silented
    // use(int, "r"), bonus part if we want to add an action attacking the Ritual card
    shared_ptr<Player> targetplayer = nullptr;
    if (p == 1) {
        targetplayer = this->master->getGame()->get1();
    } else {
        targetplayer = this->master->getGame()->get2();
    }
    
    /*if (t == "r") {
        if (targetplayer->isRitualEmpty()) {
            throw 1; // You haven't played a Ritual card!
        }
        if (name == "Banish") {
            targetplayer->getRitual() = nullptr;
        } else {
            throw 4; // The Spell cannot be played upon a Ritual!
        }
    } else {
    */
    int j;
    istringstream ss{t};
    ss >> j;
    
    if (targetplayer->getSlot()->getSize() < j) throw 3; // The target Minion DNE!
    if (name == "Novice Pyromancer") {
        shared_ptr<Minion> m = targetplayer->getSlot()->getSlot()[j-1];
        m->subDefense(1);    
    }
}

void Act_Minion::print() {
    card_template_t am = display_minion_activated_ability(this->name, this->cost, this->attack, this->defense, this->abilityCost, this->effect);
    for (auto it = am.begin(); it != am.end(); ++it) {
        cout << *it << endl;
    }
}

shared_ptr<Card> Act_Minion::clone() {
    return shared_ptr<Card>(new Act_Minion{*this});
}

shared_ptr<Minion> Act_Minion::cloneMinion() {
    return shared_ptr<Minion>(new Act_Minion{*this});
}
