#include "card.h"
#include "hand.h"
#include "minion.h"
#include "ascii_graphics.h"

using namespace std;

/*Hand::Hand(std::vector<std::shared_ptr<Card>> theHand) :
    theHand {theHand} {}
    */

Hand::~Hand() {}

vector<shared_ptr<Card>> &Hand::getHand() {return theHand;}

int Hand::getSize() {return theHand.size();}

bool Hand::isEmpty() {return theHand.size() == 0;}

bool Hand::isFull() {return theHand.size() == 5;}

void Hand::push(shared_ptr<Card> c) {theHand.emplace_back(c);}

void Hand::pop(int i) {
    theHand.erase(theHand.begin () + i - 1);
}

void Hand::print() {
    if (getSize() == 0) throw 5; // Number; There is no card in hand
    vector<card_template_t> hand_t;
    for (int j = 0; j < getSize(); j++) {
        shared_ptr<Card> c = theHand[j];
        card_template_t h;
        if (c->getType() == "Spell") {
            h = display_spell(c->getName(), c->getCost(), c->getEffect());
        } 
        else if (c->getType() == "Ritual") {
            h = display_ritual(c->getName(), c->getCost(), c->getChargeCost(), c->getEffect(), c->getCharge());
        } 
        else if (c->getType() == "Minion") {
            h = display_minion_no_ability(c->getName(), c->getCost(), c->getAttack(), c->getDefense());
        } 
        else if (c->getType() == "Act_Minion") {
            h = display_minion_activated_ability(c->getName(), c->getCost(), c->getAttack(), c->getDefense(), c->getAbilityCost(), c->getEffect());
        } 
        else if (c->getType() == "Tri_Minion") {
            h = display_minion_triggered_ability(c->getName(), c->getCost(), c->getAttack(), c->getDefense(), c->getEffect());
        } 
        else if (c->getName() == "Giant Strength") {
            h = display_enchantment_attack_defence(c->getName(), c->getCost(), c->getEffect(), "+2", "+2");
        } 
        else {
            h = display_enchantment(c->getName(), c->getCost(), c->getEffect());
        }
        hand_t.emplace_back(h);
    }
    int turn = hand_t.size() / 5;
    int left = hand_t.size() % 5;
    for (int k = 0; k < turn; ++k) {
        for (int i = 0; i < 11; ++i) {
            for (int j = 0; j < 5; ++j) {
                cout << hand_t[j][i]; 
            }
            cout << endl;
        }
    }
    if (left == 0) return;
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < left; ++j) { cout << hand_t[j][i]; }
        cout << endl;
    }
}

