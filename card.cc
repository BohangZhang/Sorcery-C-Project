#include "player.h"
#include "minion.h"
#include "card.h"

using namespace std;

Card::Card(string name, string effect, int cost, string type) :
    name{name}, effect{effect}, cost{cost}, type{type} {
    master = shared_ptr<Player>(nullptr);    
}

Card::~Card() {};

string Card::getName() {return name;}

string Card::getType() {return type;}

string Card::getEffect() {return effect;}

int Card::getCost() {return cost;}

int Card::getChargeCost() {return 0;}
int Card::getCharge() {return 0;}
int Card::getAttack() {return 0;}
int Card::getDefense() {return 0;}
string Card::getTrigger() {return "";}
int Card::getAbilityCost() {return 0;}

void Card::setMaster(shared_ptr<Player> p) {
    master = p;
}

shared_ptr<Player> Card::getMaster() {return master;}

void Card::action() {return;}

void Card::action(int p, string t) {return;}
