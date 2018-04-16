#include "player.h"
#include "ascii_graphics.h"
#include "deck.h"
#include "database.h"
#include "game.h"
#include "ritual.h"
#include "slot.h"
#include "hand.h"
#include "graveyard.h"
#include "minion.h"
#include "enchantment.h"

using namespace std;

// Big 5 and Ctor
Player::Player(string name) : name{name} {
    hp = 20;
    magic = 3;
    ritual = shared_ptr<Ritual>(nullptr);
    deck = shared_ptr<Deck>(new Deck());
    hand = shared_ptr<Hand>(new Hand());
    slot = shared_ptr<Slot>(new Slot());
    graveyard = shared_ptr<Graveyard>(new Graveyard());
    game = shared_ptr<Game>(nullptr);
    database = shared_ptr<Database>(nullptr);
}

Player::~Player() {};


// Get Info
string Player::getName() {return name;}

int Player::getHp() {return hp;}

int Player::getMagic() {return magic;}

shared_ptr<Ritual> Player::getRitual() {return ritual;}

shared_ptr<Game> Player::getGame() {return game;}

shared_ptr<Slot> Player::getSlot() {return slot;}

shared_ptr<Graveyard> Player::getGraveyard() {return graveyard;}

shared_ptr<Deck> Player::getDeck() {return deck;}

shared_ptr<Database> Player::getDatabase() {return database;}

shared_ptr<Hand> Player::getHand() {return hand;}


// Modify Fields
void Player::addMagic(int m) {this->magic += m;}

void Player::subMagic(int m) {
    if (magic >= m) magic -= m;
    else throw 6; // Number; No enough magic
}

void Player::addHp(int h) {this->hp += h;}

void Player::subHp(int h) {
    if (hp >= h) hp -= h;
}

void Player::setMagic() {
    magic = 0;
}


// Check Conditions
bool Player::isHandFull() {return this->hand->isFull();}

bool Player::isHandEmpty() {return this->hand->isEmpty();}

bool Player::isDeckEmpty() {return this->deck->isEmpty();}

bool Player::isSlotFull() {return this->slot->isFull();}

bool Player::isSlotEmpty() {return this->slot->isEmpty();}

bool Player::isGraveyardEmpty() {return this->graveyard->isEmpty();}

bool Player::isRitualEmpty() {return ritual == nullptr;}

bool Player::isDead() {return hp <= 0;}


// load Cards
void Player::setDatabase(shared_ptr<Database> d) {
    this->database = d;
}

void Player::setGame(shared_ptr<Game> g) {
    this->game = g;
}

void Player::setDeck() {
    deck = shared_ptr<Deck>(nullptr);
}
void Player::setSlot() {
    slot = shared_ptr<Slot>(nullptr);
}
void Player::setHand() {
    hand = shared_ptr<Hand>(nullptr);
}
void Player::setGraveyard() {
    graveyard = shared_ptr<Graveyard>(nullptr);
}
void Player::setRitual(shared_ptr<Ritual> r) {
    ritual = r;
}

// Actions
void Player::play(int i) { // call Game notify()
    if (getHand()->getHand().size() < i) throw 7; // Number; The card doesn't exist
    else {
        shared_ptr<Card> c = getHand()->getHand()[i - 1];
        if (getMagic() < c->getCost()) throw 6; // Number; You don't have enough Magic
        string card_type = c->getType();
        if (card_type == "Enchantment") throw 8; // Number; Wrong command
        else if (card_type == "Spell") {
            c->action();
            getHand()->pop(i);
        } else if (card_type == "Ritual") {
            ritual = dynamic_pointer_cast<Ritual>(c);
            game->notifyRitual("REnter", ritual);
            getHand()->pop(i);
        } else {
            if (isSlotFull()) {
                throw 2;
            } else {
                shared_ptr<Minion> m = dynamic_pointer_cast<Minion>(c);
                getSlot()->push(m);
                getHand()->pop(i);
                game->notify("Enter", m);
            }
        }
        subMagic(c->getCost());
        for (auto it = this->getSlot()->getSlot().begin(); it != this->getSlot()->getSlot().end(); ) {
            if ((*it)->ifDead()) {
                this->getGraveyard()->pushfirst((*it));
                this->getSlot()->pop((*it));
            } else {
                ++it;
            }
        }
        for (auto it = this->getGame()->getNoncurrent()->getSlot()->getSlot().begin(); 
             it != this->getGame()->getNoncurrent()->getSlot()->getSlot().end(); ) {
            if ((*it)->ifDead()) {
                this->getGame()->getNoncurrent()->getGraveyard()->pushfirst((*it));
                this->getGame()->getNoncurrent()->getSlot()->pop((*it));
            } else {
                ++it;
            }
        }
    }
}

void Player::play(int i, int p, string t) {
    if (getHand()->getHand().size() < i) throw 7; // Number; The card doesn't exist
    else {
        shared_ptr<Card> c = getHand()->getHand()[i - 1];
        if (getMagic() < c->getCost()) throw 6; // Number; You don't have enough Magic
        string card_type = c->getType();
        if (card_type == "Spell") { 
            c->action(p,t);
            getHand()->pop(i);
        } else if (card_type == "Enchantment") {
            if (t == "r") throw 9; // Number; Wrong command, Enchantment can't be played on Ritual
            
            shared_ptr<Player> targetplayer {nullptr};
            if (p == 1) {targetplayer = getGame()->get1();} 
            else {targetplayer = getGame()->get2();}   
    
            int j;
            istringstream ss{t};
            ss >> j;
            shared_ptr<Minion> m = targetplayer->getSlot()->getSlot()[j - 1];
            shared_ptr<Minion> h = dynamic_pointer_cast<Minion>(c);
            h->setOrigin(m);
            targetplayer->getSlot()->getSlot()[j - 1] = h;
            getHand()->pop(i);
        } else throw 10; // Number; Wrong command, this card can't be played to a target
        subMagic(c->getCost());
    }
}

void Player::testing_play(int i) { // call Game notify()
    if (getHand()->getHand().size() < i) throw 7; // Number; The card doesn't exist
    else {
        shared_ptr<Card> c = getHand()->getHand()[i - 1];
        string card_type = c->getType();
        if (card_type == "Enchantment") throw 8; // Number; Wrong command
        else if (card_type == "Spell") {
            c->action();
            getHand()->pop(i);
        } else if (card_type == "Ritual") {
            ritual = dynamic_pointer_cast<Ritual>(c);
            getHand()->pop(i);
            game->notifyRitual("REnter", ritual);
        } else {
            if (isSlotFull()) {
                throw 2;
            } else {
                shared_ptr<Minion> m = dynamic_pointer_cast<Minion>(c);
                getSlot()->push(m);
                getHand()->pop(i);
                game->notify("Enter", m);
            }
        }
        for (auto it = this->getSlot()->getSlot().begin(); it != this->getSlot()->getSlot().end(); ) {
            if ((*it)->ifDead()) {
                this->getGraveyard()->pushfirst((*it));
                this->getSlot()->pop((*it));
            } else {
                ++it;
            }
        }
        for (auto it = this->getGame()->getNoncurrent()->getSlot()->getSlot().begin(); 
             it != this->getGame()->getNoncurrent()->getSlot()->getSlot().end(); ) {
            if ((*it)->ifDead()) {
                this->getGame()->getNoncurrent()->getGraveyard()->pushfirst((*it));
                this->getGame()->getNoncurrent()->getSlot()->pop((*it));
            } else {
                ++it;
            }
        }
    }
}

void Player::testing_play(int i, int p, string t) {
    if (getHand()->getHand().size() < i) throw 7; // Number; The card doesn't exist
    else {
        shared_ptr<Card> c = getHand()->getHand()[i - 1];
        string card_type = c->getType();
        if (card_type == "Spell") { 
            c->action(p,t);
            getHand()->pop(i);
        } else if (card_type == "Enchantment") {
            if (t == "r") throw 9; // Number; Wrong command, Enchantment can't be played on Ritual
            
            shared_ptr<Player> targetplayer {nullptr};
            if (p == 1) {targetplayer = getGame()->get1();} 
            else {targetplayer = getGame()->get2();}   
    
            int j;
            istringstream ss{t};
            ss >> j;
            shared_ptr<Minion> m = targetplayer->getSlot()->getSlot()[j - 1];
            shared_ptr<Minion> h = dynamic_pointer_cast<Minion>(c);
            h->setOrigin(m);
            targetplayer->getSlot()->getSlot()[j - 1] = h;
            getHand()->pop(i);
        } else throw 10; // Number; Wrong command, this card can't be played to a target
    }
}

void Player::use(int i) {
    if (getSlot()->getSize() < i) throw 11; // Number; The Minion doesn't exist
    shared_ptr<Minion> m = getSlot()->getSlot()[i - 1];
    string minion_type = m->getType();
    if (minion_type == "Enchantment") {
        minion_type = m->getOriginType();
    }
    if (minion_type != "Act_Minion") throw 12; // Number; Wrong command, the minion doesn't have an ability to use
    if (getMagic() < m->getAbilityCost()) throw 6; // Number; You don't have enough Magic
    if (m->getSilent() == 1) throw 1; // The Ac Minion has been silented
    m->use();
    subMagic(m->getAbilityCost());
}

void Player::use(int i, int p, string t) {
    if (getSlot()->getSize() < i) throw 11; // Number; The Minion doesn't exist
    shared_ptr<Minion> m = getSlot()->getSlot()[i - 1];
    string minion_type = m->getType();
    if (minion_type == "Enchantment") {
        minion_type = m->getOriginType();
    }
    if (minion_type != "Act_Minion") throw 12; // Number; Wrong command, the minion doesn't have an ability
    if (getMagic() < m->getAbilityCost()) throw 6; // Number; You don't have enough Magic
    if (m->getSilent() == 1) throw 1; // The Ac Minion has been silented
    m->use(p,t);
    subMagic(m->getAbilityCost());
    for (auto it = this->getSlot()->getSlot().begin(); it != this->getSlot()->getSlot().end(); ) {
        if ((*it)->ifDead()) {
            this->getGraveyard()->pushfirst((*it));
            this->getSlot()->pop((*it));
        } else {
            ++it;
        }
    }
    for (auto it = this->getGame()->getNoncurrent()->getSlot()->getSlot().begin(); 
        it != this->getGame()->getNoncurrent()->getSlot()->getSlot().end(); ) {
        if ((*it)->ifDead()) {
            this->getGame()->getNoncurrent()->getGraveyard()->pushfirst((*it));
            this->getGame()->getNoncurrent()->getSlot()->pop((*it));
        } else {
            ++it;
        }
    }
}

void Player::attack(int i) {
    if (getSlot()->getSize() < i) throw 11; // Number; The Minion doesn't exist
    shared_ptr<Minion> m = getSlot()->getSlot()[i - 1];
    m->attackPlayer();
/*  string type = m->getType();
    if (m->ifDead()) {
        if (type == "Enchantment") {
            shared_ptr<Minion> dead_m = getDatabase()->getMinion(m->getOriginName());
            getGraveyard()->pushfirst(dead_m);
        }
        else {
            shared_ptr<Minion> dead_m = getDatabase()->getMinion(m->getName());
            getGraveyard()->pushfirst(dead_m);
        }
        getSlot()->pop(m);
    } */
}

void Player::attack(int i, int j) {
    shared_ptr<Player> nonCurrent = getGame()->getNoncurrent();
    if (getSlot()->getSize() < i) throw 11; // Number; The Minion doesn't exist
    if (nonCurrent->getSlot()->getSize() < j) throw 3; // Number; The opponent doesn't exist  
    shared_ptr<Minion> m1 = getSlot()->getSlot()[i - 1];
    shared_ptr<Minion> m2 = nonCurrent->getSlot()->getSlot()[j - 1];
    m1->attackMinion(j - 1);
    for (auto it = this->getSlot()->getSlot().begin(); it != this->getSlot()->getSlot().end(); ) {
        if ((*it)->ifDead()) {
            this->getGraveyard()->pushfirst((*it));
            this->getSlot()->pop((*it));            
        } else {
            ++it;
        }
    }
    for (auto it = this->getGame()->getNoncurrent()->getSlot()->getSlot().begin(); 
         it != this->getGame()->getNoncurrent()->getSlot()->getSlot().end(); ) {
        if ((*it)->ifDead()) {
            this->getGame()->getNoncurrent()->getGraveyard()->pushfirst((*it));
            this->getGame()->getNoncurrent()->getSlot()->pop((*it));
        } else {
            ++it;
        }
    }
}

void Player::notify(string t, shared_ptr<Minion> m) {
    shared_ptr<Slot> s = this->slot;
    if (t == "Start" || t == "End") {
        for (auto it = s->getSlot().begin(); it != s->getSlot().end(); ++it) {
            if ((*it)->getType() == "Enchantment") {
                if (((*it)->getOriginType() == "Tri_Minion") && ((*it)->getTriggerType() == t) && ((*it)->getSilent() == 0)) {
                    (*it)->action();
                    continue;
                }
            } else {
                if (((*it)->getType() == "Tri_Minion") && ((*it)->getTrigger() == t) && ((*it)->getSilent() == 0)) {
                    (*it)->action();
                    continue;
                }
            }
        }
        if ((!(this->isRitualEmpty())) && (this->getRitual()->isUsable()) && (this->getRitual()->getTrigger() == t)) {
            this->getRitual()->action();
            this->getRitual()->subCharge(this->getRitual()->getChargeCost());
        }
    }
    if (t == "MEnter" || t == "OEnter") {
        for (auto it = s->getSlot().begin(); it != s->getSlot().end(); ++it) {
            if ((*it)->getType() == "Enchantment") {
                if (((*it)->getOriginType() == "Tri_Minion") && (((*it)->getTriggerType() == t) || ((*it)->getTriggerType() == "Enter")) && ((*it)->getSilent() == 0)) {
                    (*it)->action(m);
                    if (m->ifDead()) {
                        m->getMaster()->getGraveyard()->pushfirst(m);
                        m->getMaster()->getSlot()->pop(m);
                        return;
                    }
                    continue;
                }
            } else {
                if (((*it)->getType() == "Tri_Minion") && (((*it)->getTrigger() == t) || ((*it)->getTrigger() == "Enter")) && ((*it)->getSilent() == 0)) {
                    (*it)->action(m);
                    if (m->ifDead()) {
                        m->getMaster()->getGraveyard()->pushfirst(m);
                        m->getMaster()->getSlot()->pop(m);
                        return;
                    }
                    continue;
                }
            }
        }
        if ((!(this->isRitualEmpty())) && (this->getRitual()->isUsable()) && ((this->getRitual()->getTrigger() == t) || (this->getRitual()->getTrigger() == "Enter"))) {
            this->getRitual()->action(m);
            this->getRitual()->subCharge(this->getRitual()->getChargeCost());
        }
    }
    for (auto it = this->getSlot()->getSlot().begin(); it != this->getSlot()->getSlot().end(); ) {
        if ((*it)->ifDead()) {
            this->getGraveyard()->pushfirst((*it));
            this->getSlot()->pop((*it));            
        } else {
            ++it;
        }
    }
    for (auto it = this->getGame()->getNoncurrent()->getSlot()->getSlot().begin(); 
         it != this->getGame()->getNoncurrent()->getSlot()->getSlot().end(); ) {
        if ((*it)->ifDead()) {
            this->getGame()->getNoncurrent()->getGraveyard()->pushfirst((*it));
            this->getGame()->getNoncurrent()->getSlot()->pop((*it));
        } else {
            ++it;
        }
    }
}

void Player::notifyRitual(string t, shared_ptr<Ritual> r) {
    shared_ptr<Slot> s = this->slot;
    for (auto it = s->getSlot().begin(); it != s->getSlot().end(); ++it) {
        if ((*it)->getType() == "Enchantment") {
            if (((*it)->getOriginType() == "Tri_Minion") && ((*it)->getTriggerType() == t) && ((*it)->getSilent() == 0)) {
                (*it)->actionRitual(r);
                continue;
            }
        } else {
            if (((*it)->getType() == "Tri_Minion") && ((*it)->getTrigger() == t) && ((*it)->getSilent() == 0)) {
                (*it)->actionRitual(r);
                continue;
            }
        }
    }
}

// Command
void Player::inspect(int i) {
    shared_ptr<Slot> s = this->slot;
    if (s->getSize() < i) {
        throw 11; // Number, You don't have enough minion
    }
    shared_ptr<Minion> m = s->getSlot()[i-1];
    shared_ptr<Minion> am = s->getSlot()[i-1];
    shared_ptr<vector<card_template_t>> temp {new vector<card_template_t>};
    while (1) {
        if ((m->getType() == "Minion") || (m->getType() == "Act_Minion") || (m->getType() == "Tri_Minion")) {
            am->print();
            break;
        } else {
            if (m->getName() == "Giant Strength") {
                temp->push_back(display_enchantment_attack_defence(m->getName(), m->getCost(), m->getEffect(), "+2", "+2"));
            } else {
                temp->push_back(display_enchantment(m->getName(), m->getCost(), m->getEffect()));
            }
            m = m->getOrigin();
        }
    }
    if (temp->size() == 0) {return;}
    int turn = temp->size() / 5;
    int left = temp->size() % 5;
    for (int k = 0; k < turn; ++k) {
        for (int i = 0; i < 11; ++i) {
            for (int j = 0; j < 5; ++j) { cout << (*temp)[j][i]; }
            cout << endl;
        }
    }
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < left; ++j) { cout << (*temp)[j][i]; }
        cout << endl;
    }
}

void Player::draw() {
    if (this->isDeckEmpty()) {
        throw 4; // Number, You deck is empty!
    } else if (this->isHandFull()) {
        throw 13; // Number, You hand is full!
    } else {
        shared_ptr<Card> c = deck->getDeck()[0];
        this->deck->popfirst();
        this->hand->push(c);
    }
}

// Print
void Player::handDisplay() {hand->print();}

//minion_slot_helper

void Player::minion_slot_print () {
    for (int i = 0; i < 11; i++) {
        cout << EXTERNAL_BORDER_CHAR_UP_DOWN;
        // print minion slot
        for (int j = 0; j < 5; j++) {
            if (j < slot->getSize()) {
                if (slot->getSlot()[j]->getType() == "Act_Minion") {
                    card_template_t active_m = display_minion_activated_ability(slot->getSlot()[j]->getName(), slot->getSlot()[j]->getCost(),slot->getSlot()[j]->getAttack(), slot->getSlot()[j]->getDefense(),  
                        slot->getSlot
                        ()[j]->getAbilityCost(), slot->getSlot()[j]->getEffect());
                    cout << active_m[i];
                }
                else if (slot->getSlot()[j]->getType() == "Tri_Minion") {
                    card_template_t tri_m = display_minion_triggered_ability(slot->getSlot()[j]->getName(), slot->getSlot()[j]->getCost(),slot->getSlot()[j]->getAttack(), slot->getSlot()[j]->getDefense(),     
                        slot->getSlot()[j]->getEffect());
                    cout << tri_m[i];
                }
                else if (slot->getSlot()[j]->getType() == "Enchantment") {
                    if (slot->getSlot()[j]->getOriginType() == "Act_Minion") {
                        card_template_t active_m = display_minion_activated_ability(slot->getSlot()[j]->getOriginName(), slot->getSlot()[j]->getOriginCost(),slot->getSlot()[j]->getAttack(), slot->getSlot()[j]->getDefense(),  
                            slot->getSlot()[j]->getAbilityCost(), slot->getSlot()[j]->getOriginEffect());
                        cout << active_m[i];
                    }
                    else if (slot->getSlot()[j]->getOriginType() == "Tri_Minion") {
                        card_template_t tri_m = display_minion_triggered_ability(slot->getSlot()[j]->getOriginName(), slot->getSlot()[j]->getOriginCost(),slot->getSlot()[j]->getAttack(), slot->getSlot()[j]->getDefense(),     
                            slot->getSlot()[j]->getOriginEffect());
                        cout << tri_m[i];
                    }
                    else {
                        card_template_t m = display_minion_no_ability(slot->getSlot()[j]->getOriginName(), slot->getSlot()[j]->getOriginCost(),slot->getSlot()[j]->getAttack(), slot->getSlot()[j]->getDefense());
                        cout << m[i];
                    }
                }
                else {
                    card_template_t m = display_minion_no_ability(slot->getSlot()[j]->getName(), slot->getSlot()[j]->getCost(),slot->getSlot()[j]->getAttack(), slot->getSlot()[j]->getDefense());
                    cout << m[i];
                }
            }
            else {
                cout << CARD_TEMPLATE_BORDER[i];
            }
        }
        cout << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
    }
}


void Player::boardDisplay(int n) {
    // variables declared
    card_template_t player1 = display_player_card(1, name, hp, magic);
    card_template_t player2 = display_player_card(2, name, hp, magic);

    // case 1: print the board of player 1
    // display ritual, player, graveyard -- the first 11 lines 
    if (n == 1) {
    // Print the top board line
        cout << EXTERNAL_BORDER_CHAR_TOP_LEFT;
        for (int i = 0; i < 165; ++i) {
            cout << EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
        }
        cout << EXTERNAL_BORDER_CHAR_TOP_RIGHT << endl;
        for (int i = 0; i < 11; i++) {
            // if graveyard and ritual are empty
            if ((isGraveyardEmpty()) && (isRitualEmpty())) {
                cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << CARD_TEMPLATE_BORDER[i] << EXTERNAL_BORDER_CHAR_UP_DOWN << endl;
            }
            else if (isGraveyardEmpty()) {
                card_template_t r = display_ritual(ritual->getName(), ritual->getCost(), ritual->getChargeCost(), ritual->getEffect(), ritual->getCharge());
                cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << CARD_TEMPLATE_BORDER[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
            }
            else if (isRitualEmpty()) {
                if ( graveyard->top()->getType() == "Act_Minion") {
                    card_template_t active_m = display_minion_activated_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                        graveyard->top()->getAbilityCost(), graveyard->top()->getEffect());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << active_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
                else if ( graveyard->top()->getType() == "Tri_Minion") {
                    card_template_t tri_m = display_minion_triggered_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                        graveyard->top()->getEffect());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << tri_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
                else if ( graveyard->top()->getType() == "Enchantment") {
                    if ( graveyard->top()->getOriginType() == "Act_Minion") {
                        card_template_t active_m = display_minion_activated_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                            graveyard->top()->getAbilityCost(), graveyard->top()->getOriginEffect());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << active_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                    else if ( graveyard->top()->getType() == "Tri_Minion") {
                        card_template_t tri_m = display_minion_triggered_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                            graveyard->top()->getOriginEffect());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << tri_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                    else {
                        card_template_t m = display_minion_no_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                }
                else {
                    card_template_t m = display_minion_no_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
            }
            else {
                // neither ritual, player, graveyard is empty
                card_template_t r = display_ritual(ritual->getName(), ritual->getCost(), ritual->getChargeCost(), ritual->getEffect(), ritual->getCharge());
                if ( graveyard->top()->getType() == "Act_Minion") {
                    card_template_t active_m = display_minion_activated_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                        graveyard->top()->getAbilityCost(), graveyard->top()->getEffect());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << active_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
                else if ( graveyard->top()->getType() == "Tri_Minion") {
                    card_template_t tri_m = display_minion_triggered_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                        graveyard->top()->getEffect());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << tri_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
                else if ( graveyard->top()->getType() == "Enchantment") {
                    if ( graveyard->top()->getOriginType() == "Act_Minion") {
                        card_template_t active_m = display_minion_activated_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                            graveyard->top()->getAbilityCost(), graveyard->top()->getOriginEffect());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << active_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                    else if ( graveyard->top()->getType() == "Tri_Minion") {
                        card_template_t tri_m = display_minion_triggered_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                            graveyard->top()->getOriginEffect());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << tri_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                    else {
                        card_template_t m = display_minion_no_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                }
                else {
                    card_template_t m = display_minion_no_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
            }
        }
    // print the minion slot of player1
        minion_slot_print();
    }

// case 2: print the board of player 2
    else {
        minion_slot_print();
        for (int i = 0; i < 11; i++) {
            // if graveyard and ritual are empty
            if ((isGraveyardEmpty()) && (isRitualEmpty())) {
                cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER[i] << CARD_TEMPLATE_EMPTY[i] << player2[i] << CARD_TEMPLATE_EMPTY[i] << CARD_TEMPLATE_BORDER[i] << EXTERNAL_BORDER_CHAR_UP_DOWN << endl;
            }
            else if (isGraveyardEmpty()) {
                card_template_t r = display_ritual(ritual->getName(), ritual->getCost(), ritual->getChargeCost(), ritual->getEffect(), ritual->getCharge());
                cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player2[i] << CARD_TEMPLATE_EMPTY[i] << CARD_TEMPLATE_BORDER[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
            }
            else if (isRitualEmpty()) {
                if ( graveyard->top()->getType() == "Act_Minion") {
                    card_template_t active_m = display_minion_activated_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                        graveyard->top()->getAbilityCost(), graveyard->top()->getEffect());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << active_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
                else if ( graveyard->top()->getType() == "Tri_Minion") {
                    card_template_t tri_m = display_minion_triggered_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                        graveyard->top()->getEffect());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << tri_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
                else if ( graveyard->top()->getType() == "Enchantment") {
                    if ( graveyard->top()->getOriginType() == "Act_Minion") {
                        card_template_t active_m = display_minion_activated_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                            graveyard->top()->getAbilityCost(), graveyard->top()->getOriginEffect());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << active_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                    else if ( graveyard->top()->getType() == "Tri_Minion") {
                        card_template_t tri_m = display_minion_triggered_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                            graveyard->top()->getOriginEffect());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << tri_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                    else {
                        card_template_t m = display_minion_no_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                }
                else {
                    card_template_t m = display_minion_no_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << CARD_TEMPLATE_BORDER [i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
            }
            else {
                // neither ritual, player, graveyard is empty
                card_template_t r = display_ritual(ritual->getName(), ritual->getCost(), ritual->getChargeCost(), ritual->getEffect(), ritual->getCharge());
                if ( graveyard->top()->getType() == "Act_Minion") {
                    card_template_t active_m = display_minion_activated_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                        graveyard->top()->getAbilityCost(), graveyard->top()->getEffect());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << active_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
                else if ( graveyard->top()->getType() == "Tri_Minion") {
                    card_template_t tri_m = display_minion_triggered_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                        graveyard->top()->getEffect());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << tri_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
                else if ( graveyard->top()->getType() == "Enchantment") {
                    if ( graveyard->top()->getOriginType() == "Act_Minion") {
                        card_template_t active_m = display_minion_activated_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                            graveyard->top()->getAbilityCost(), graveyard->top()->getOriginEffect());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << active_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                    else if ( graveyard->top()->getType() == "Tri_Minion") {
                        card_template_t tri_m = display_minion_triggered_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense(),
                            graveyard->top()->getOriginEffect());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << tri_m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                    else {
                        card_template_t m = display_minion_no_ability(graveyard->top()->getOriginName(), graveyard->top()->getOriginCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense());
                        cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                    }
                }
                else {
                    card_template_t m = display_minion_no_ability(graveyard->top()->getName(), graveyard->top()->getCost(),graveyard->top()->getAttack(), graveyard->top()->getDefense());
                    cout << EXTERNAL_BORDER_CHAR_UP_DOWN << r[i] << CARD_TEMPLATE_EMPTY[i] << player1[i] << CARD_TEMPLATE_EMPTY[i] << m[i] << EXTERNAL_BORDER_CHAR_UP_DOWN <<endl;
                }
            }
        }
// print buttom board line
        cout << EXTERNAL_BORDER_CHAR_BOTTOM_LEFT;
        for (int i = 0; i < 165; ++i) {
            cout << EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
        }
        cout << EXTERNAL_BORDER_CHAR_BOTTOM_RIGHT << endl;
    }
}

