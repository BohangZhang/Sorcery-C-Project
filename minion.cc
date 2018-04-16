#include "minion.h"
#include "player.h"
#include "game.h"
#include "slot.h"
#include <iostream>
using namespace std;

Minion::Minion(string name, int cost, string type, int attack, int defense, string effect) : 
    Card{name, effect, cost, type}, attack{attack}, defense{defense} {
    silent = 0;    
}

Minion::~Minion() {}

int Minion::getAttack() {return attack;}

int Minion::getDefense() {return defense;}

bool Minion::getSilent() {return 0;}

int Minion::getAbilityCost() {return 0;}

string Minion::getTrigger() {return "Empty";}

shared_ptr<Minion> Minion::getOrigin() {return nullptr;}

string Minion::getOriginName() {return "";}

string Minion::getOriginEffect() {return "";}

int Minion::getOriginCost() {return 0;}

string Minion::getOriginType() {return "";}

shared_ptr<Player> Minion::getOriginMaster() {return nullptr;}

string Minion::getTriggerType() {return "";}

void Minion::addAttack(int a) {attack += a;}

void Minion::addDefense(int d) {defense += d;}

void Minion::subAttack(int a) {attack -= a;}

void Minion::subDefense(int d) {defense -= d;}

bool Minion::ifDead() {return getDefense() <= 0;}

void Minion::attackPlayer() {
    this->getMaster()->getGame()->getNoncurrent()->subHp(getAttack());    
}

void Minion::attackMinion(int j) {
    vector<shared_ptr<Minion>> s = this->master->getGame()->getNoncurrent()->getSlot()->getSlot();
    s[j]->subDefense(this->getAttack());
    this->subDefense(s[j]->getAttack());
}

void Minion::use() {return;}
void Minion::use(int i, string s) {return;}
void Minion::action(shared_ptr<Minion> m) {return;}
void Minion::actionRitual(shared_ptr<Ritual> r) {return;}
void Minion::setOrigin(shared_ptr<Minion> m) {return;}

void Minion::print() {
    card_template_t m = display_minion_no_ability(this->name, this->cost, getAttack(), getDefense());
    for (auto it = m.begin(); it != m.end(); ++it) {
        cout << *it << endl;
    }
}

shared_ptr<Card> Minion::clone() {
    return shared_ptr<Card>(new Minion{*this});
}

shared_ptr<Minion> Minion::cloneMinion() {
    return shared_ptr<Minion>(new Minion{*this});
}
