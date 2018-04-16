#include "player.h"
#include "ritual.h"
#include "card.h"
#include "minion.h"
#include "graveyard.h"
#include "slot.h"

using namespace std;

Ritual::Ritual(string name, string effect, int cost, string type, int charge, int chargecost, string trigger) : 
    Card{name, effect, cost, type}, charge{charge}, chargecost{chargecost}, trigger{trigger} {}
    
Ritual::~Ritual() {}

int Ritual::getCharge() {return charge;}

int Ritual::getChargeCost() {return chargecost;}

string Ritual::getTrigger() {return trigger;}

void Ritual::addCharge(int c) {charge += c;}

void Ritual::subCharge(int c) {this->charge -= c;}

bool Ritual::isUsable() {
    return charge >= chargecost;
}

void Ritual::action() {
    this->master->addMagic(1);
}

void Ritual::action(shared_ptr<Minion> m) {
    if (name == "Aura of Power") {
        m->addAttack(1);
        m->addDefense(1);
    }
    if (name == "Standstill") {
        m->getMaster()->getSlot()->pop(m);
        m->getMaster()->getGraveyard()->pushfirst(m);
    }
}

void Ritual::print() {
    card_template_t r = display_ritual(this->name, this->cost, this->chargecost, this->effect, this->charge);
    for (auto it = r.begin(); it != r.end(); ++it) {
        cout << *it << endl;
    }
}

shared_ptr<Card> Ritual::clone() {
    return shared_ptr<Card>(new Ritual{*this});
}

