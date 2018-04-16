#include "spell.h"
#include "ascii_graphics.h"
#include "card.h"
#include "player.h"
#include "ritual.h"
#include "slot.h"
#include "minion.h"
#include "game.h"
#include "graveyard.h"
#include "deck.h"
#include "hand.h"
#include "database.h"

using namespace std;

Spell::Spell(string name, string effect, int cost, string type) : 
    Card{name, effect, cost, type} {}

Spell::~Spell() {}

void Spell::action() {
    if (name == "Recharge") {
        if (this->master->isRitualEmpty()) {
            throw 15; // You haven't played a Ritual card!
        } else {
            this->master->getRitual()->addCharge(3);
        }
        return;
    }
    if (name == "Blizzard") {
        for (auto it = this->master->getSlot()->getSlot().begin(); it != this->master->getSlot()->getSlot().end(); ++it) {
            (*it)->subDefense(2);
        }
        for (auto it = this->master->getGame()->getNoncurrent()->getSlot()->getSlot().begin(); 
             it != this->master->getGame()->getNoncurrent()->getSlot()->getSlot().end(); ++it) {
            (*it)->subDefense(2);
        }
        return;
    }
    if (name == "Raise Dead") {
        if (this->master->isGraveyardEmpty()) {
            throw 16; // Your Graveyard is empty!
        } else {
            shared_ptr<Minion> p = this->master->getGraveyard()->top();
            if (this->master->isSlotFull()) {
                throw 2; // You have already own 5 Minions!
            } else {
                this->master->getGraveyard()->popfirst();
                this->master->getSlot()->push(p);
            }
        }
        return;
    }
    throw 10; // You use the wrong command
}

void Spell::action(int p, string t) {
    shared_ptr<Player> targetplayer = nullptr;
    if (p == 1) {
        targetplayer = this->master->getGame()->get1();
    } else {
        targetplayer = this->master->getGame()->get2();
    }
    
    if (t == "r") {
        if (targetplayer->isRitualEmpty()) {
            throw 15; // You haven't played a Ritual card!
        }
        if (name == "Banish") {
            targetplayer->setRitual(shared_ptr<Ritual>(nullptr));
        } else {
            throw 17; // The Spell cannot be played upon a Ritual!
        }
    } else {
        int j;
        istringstream ss{t};
        ss >> j;
        if (targetplayer->getSlot()->getSize() < j) {
            throw 3; // The target Minion DNE!
        }
        if (name == "Unsummon") {
            shared_ptr<Slot> s = targetplayer->getSlot();
            shared_ptr<Minion> m = s->getSlot()[j-1];
            if (m->getType() == "Enchantment") {
                shared_ptr<Card> p = m->getMaster()->getDatabase()->getCard(m->getOriginName())->clone();
                p->setMaster(m->getMaster());
                m->getMaster()->getDeck()->push(p);
                s->pop(m);
            } else {
                shared_ptr<Card> p = m->getMaster()->getDatabase()->getCard(m->getName())->clone();
                p->setMaster(m->getMaster());
                m->getMaster()->getDeck()->push(p);
                s->pop(m);
            }
            return;
        }
        if (name == "Banish") {
            shared_ptr<Slot> s = targetplayer->getSlot();
            targetplayer->getGraveyard()->pushfirst(s->getSlot()[j-1]);
            s->getSlot().erase(s->getSlot().begin()+j-1);
            return;
        }
        if (name == "Disenchant") {
            shared_ptr<Minion> m = targetplayer->getSlot()->getSlot()[j-1];
            if (m->getType() == "Enchantment") {
                targetplayer->getSlot()->getSlot()[j-1] = m->getOrigin();
            } else {
                throw 18; // Number, Target Minion hasn't been enchanted.
            }
            return;
        }
        throw 10; // Number, Wrong Spell
    }
}

void Spell::print() {
    card_template_t s = display_spell(this->name, this->cost, this->effect);
    for (auto it = s.begin(); it != s.end(); ++it) {
        cout << *it << endl;
    }
}

//void Spell::notify(string trigger) {
//    
//}

shared_ptr<Card> Spell::clone() {
    return shared_ptr<Card>(new Spell{*this});
}
