#include "tri_minion.h"
#include "player.h"
#include "card.h"
#include "database.h"
#include "slot.h"
#include "game.h"
#include <sstream>
#include "ritual.h"

using namespace std;

Tri_Minion::Tri_Minion(string name, int cost, string type, int attack, int defense, string effect, string trigger) :
    Minion{name, cost, type, attack, defense, effect}, trigger{trigger} {}

Tri_Minion::~Tri_Minion() {}

string Tri_Minion::getTrigger() {return trigger;}

void Tri_Minion::action(shared_ptr<Minion> m) {
    if (name == "Fire Elemental") {
        m->subDefense(1);
    }
    if (name == "Potion Seller") {
        for (auto it = this->master->getSlot()->getSlot().begin(); it != this->master->getSlot()->getSlot().end(); ++it) {
            (*it)->addDefense(1);
        }
    }
}

void Tri_Minion::actionRitual(shared_ptr<Ritual> r) {
    if (name == "Lushman") {
        r->getMaster()->setRitual();
    }
}

void Tri_Minion::print() {
    card_template_t tm = display_minion_triggered_ability(this->name, this->cost, this->attack, this->defense, this->effect);
    for (auto it = tm.begin(); it != tm.end(); ++it) {
        cout << *it << endl;
    }
}

shared_ptr<Card> Tri_Minion::clone() {
    return shared_ptr<Card>(new Tri_Minion{*this});
}

shared_ptr<Minion> Tri_Minion::cloneMinion() {
    return shared_ptr<Minion>(new Tri_Minion{*this});
}
