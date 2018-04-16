#include "card.h"
#include "enchantment.h"
#include "player.h"
#include "database.h"
#include "slot.h"
#include "game.h"
#include <sstream>

using namespace std;


// Enchantment
Enchantment::Enchantment(string name, int cost, string type, int attack, int defense, string effect) :
    Minion{name, cost, type, attack, defense, effect} {
        origin = shared_ptr<Minion>(nullptr);
}
    
Enchantment::~Enchantment() {}

shared_ptr<Minion> Enchantment::getOrigin() {return origin;}

string Enchantment::getOriginName() {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->getName();
    return getOrigin()->getOriginName();
}

string Enchantment::getOriginEffect() {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->getEffect();
    return getOrigin()->getOriginEffect();
}

int Enchantment::getOriginCost() {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->getCost();
    return getOrigin()->getOriginCost();
}

string Enchantment::getOriginType() {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->getType();
    return getOrigin()->getOriginType();
}

shared_ptr<Player> Enchantment::getOriginMaster() {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->getMaster();
    return getOrigin()->getOriginMaster();
}

string Enchantment::getTriggerType() {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->getTrigger();
    return getOrigin()->getTriggerType();
}

void Enchantment::addAttack(int i) {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->addAttack(i);
    return getOrigin()->addAttack(i);
}

void Enchantment::addDefense(int i) {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->addDefense(i);
    return getOrigin()->addDefense(i);
}

void Enchantment::subAttack(int i) {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->subAttack(i);
    return getOrigin()->subAttack(i);
}

void Enchantment::subDefense(int i) {
    if (getOrigin()->getType() != "Enchantment") return getOrigin()->subDefense(i);
    return getOrigin()->subDefense(i);
}

bool Enchantment::ifDead() {return this->getDefense() <= 0;}

void Enchantment::use() {
    if (getOrigin()->getType() == "Act_Minion") return getOrigin()->use();
    return getOrigin()->use();
}

void Enchantment::use(int i, string t) {
    if (getOrigin()->getType() == "Act_Minion") return getOrigin()->use(i,t);
    return getOrigin()->use(i,t);
}
 
void Enchantment::attackPlayer()  {
    this->getMaster()->getGame()->getNoncurrent()->subHp(this->getAttack());    
}

void Enchantment::attackMinion(int j)  {
    vector<shared_ptr<Minion>> s = this->master->getGame()->getNoncurrent()->getSlot()->getSlot();
    if (s.size() > j) {
        s[j]->subDefense(this->getAttack());
        this->subDefense(s[j]->getAttack());
    }
    else throw 3; // Number; The opponent player doesn't have the ith minion
}

void Enchantment::action(shared_ptr<Minion> m) {
    if (getOrigin()->getType() == "Tri_Minion") return getOrigin()->action();
    return getOrigin()->action();
}

void Enchantment::setOrigin(shared_ptr<Minion> m) {
    origin = m;
}

void Enchantment::print() {
    if (this->getOriginType() == "Act_Minion") {
        card_template_t s = display_minion_activated_ability(this->getOriginName(), this->getOriginCost(), this->getAttack(), this->getDefense(), this->getAbilityCost(), this->getOriginEffect());
        for (auto it = s.begin(); it != s.end(); ++it) {
            cout << *it << endl;
        }
    }
    else if (this->getOriginType() == "Tri_Minion") {
        card_template_t s = display_minion_triggered_ability(this->getOriginName(), this->getOriginCost(), this->getAttack(), this->getDefense(), this->getOriginEffect());
        for (auto it = s.begin(); it != s.end(); ++it) {
            cout << *it << endl;
        }
    } else {
        card_template_t s = display_minion_no_ability(this->getOriginName(), this->getOriginCost(), this->getAttack(), this->getDefense());
        for (auto it = s.begin(); it != s.end(); ++it) {
            cout << *it << endl;
        }
    }
    return;
}

shared_ptr<Card> Enchantment::clone() {
    return shared_ptr<Card>(new Enchantment{*this});
}

std::shared_ptr<Minion> Enchantment::cloneMinion() {
    return shared_ptr<Minion>(nullptr);
}


// GiantStrength

GiantStrength::GiantStrength(string name, int cost, string type, int attack, int defense, string effect) :
    Enchantment{name, cost, type, attack, defense, effect} {}
    
GiantStrength::~GiantStrength() {}

int GiantStrength::getAttack() {return getOrigin()->getAttack() + 2;}

int GiantStrength::getDefense() {return getOrigin()->getDefense() + 2;}

int GiantStrength::getAbilityCost() {return getOrigin()->getAbilityCost();}

bool GiantStrength::getSilent() {return getOrigin()->getSilent();}

shared_ptr<Card> GiantStrength::clone() {
    return shared_ptr<Card>(new GiantStrength{*this});
}

std::shared_ptr<Minion> GiantStrength::cloneMinion() {
    return getOrigin()->cloneMinion();
}

// MagicFatigue

MagicFatigue::MagicFatigue(string name, int cost, string type, int attack, int defense, string effect) :
    Enchantment{name, cost, type, attack, defense, effect} {}

MagicFatigue::~MagicFatigue() {}

int MagicFatigue::getAttack() {return getOrigin()->getAttack();}

int MagicFatigue::getDefense() {return getOrigin()->getDefense();}

int MagicFatigue::getAbilityCost() {return getOrigin()->getAbilityCost() + 2;}

bool MagicFatigue::getSilent() {return getOrigin()->getSilent();}

shared_ptr<Card> MagicFatigue::clone() {
    return shared_ptr<Card>(new MagicFatigue{*this});
}

std::shared_ptr<Minion> MagicFatigue::cloneMinion() {
    return getOrigin()->cloneMinion();
}

// Silence

Silence::Silence(string name, int cost, string type, int attack, int defense, string effect) :
    Enchantment{name, cost, type, attack, defense, effect} {}

Silence::~Silence() {}

int Silence::getAttack() {return getOrigin()->getAttack();}

int Silence::getDefense() {return getOrigin()->getDefense();}

int Silence::getAbilityCost() {return getOrigin()->getAbilityCost();}

bool Silence::getSilent() {return 1;}

shared_ptr<Card> Silence::clone() {
    return shared_ptr<Card>(new Silence{*this});
}

std::shared_ptr<Minion> Silence::cloneMinion() {
    return getOrigin()->cloneMinion();
}
