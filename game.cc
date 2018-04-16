#include "act_minion.h"
#include "card.h"
#include "enchantment.h"
#include "graveyard.h"
#include "hand.h"
#include "ritual.h"
#include "spell.h"
#include "player.h"
#include "game.h"
#include "database.h"
#include "deck.h"
#include "ascii_graphics.h"
#include "minion.h"
#include "slot.h"
#include "deck.h"
#include "tri_minion.h"
#include <fstream>
#include <sstream>

using namespace std;


// Big 5 and Ctor
Game::Game(shared_ptr<Player> player1, shared_ptr<Player> player2) : player1{player1}, player2{player2} {
    curplayer = player1;
    noncurplayer = player2;
    Round = 1;
    eog = false;
}

Game::~Game() {};

// Get Info
int Game::getRound() {return Round;}

shared_ptr<Player> Game::getCurrent() {return curplayer;}

shared_ptr<Player> Game::getNoncurrent() {return noncurplayer;}

shared_ptr<Player> Game::get1() {return player1;}

shared_ptr<Player> Game::get2() {return player2;}
    
    
// Modify Fields
void Game::addCard(int i) {
    if (i == 1) {
        this->player1->draw();
    } else {
        this->player2->draw();
    }
}
    
void Game::addMagic(int i, int m) {
    if (i == 1) {
        this->player1->addMagic(m);
    } else {
        this->player2->addMagic(m);
    }
}
    

// Conditions
void Game::nextTurn() {
    notify("End");
    shared_ptr<Player> p = curplayer;
    curplayer = noncurplayer;
    noncurplayer = p;
    ++Round;
    if (Round % 2 == 0) {
        addMagic(2, 1);
        addCard(2);
    } else {
        addMagic(1, 1);
        addCard(1);
    }
    notify("Start");
} 
    
bool Game::gameOver() {
    return (curplayer->isDead()||noncurplayer->isDead());
}
    
// load Cards
void Game::loadDecks(ifstream &deck1, ifstream &deck2) {
    string cardname;
    shared_ptr<Card> newcard{nullptr};
    while (getline(deck1, cardname)) {
        if (deck1.fail()) {break;}
        newcard = this->get1()->getDatabase()->getCard(cardname);
        shared_ptr<Card> c = newcard->clone();
        c->setMaster(player1);
        this->get1()->getDeck()->push(c);
    }
    while (getline(deck2, cardname)) {
        if (deck2.fail()) {break;}
        newcard = this->get2()->getDatabase()->getCard(cardname);
        shared_ptr<Card> c = newcard->clone();
        c->setMaster(player2);
        this->get2()->getDeck()->push(c);
    }
}

void Game::loadDatabase() {
    shared_ptr<Database> d{new Database};
    ifstream f{"Database.data"};
    
    string onecard;
    
    string name;
    string type;
    string coststring;
    int cost;
    string effect;
    string attacks, defenses, usecosts;
    int attack, defense, usecost;
    string trigger;
    while (getline(f, onecard)) {
        if (f.fail()) {break;}
        istringstream ss{onecard};
        getline(ss, name, ';');
        getline(ss, type, ';');
        getline(ss, coststring, ';');
        istringstream temp{coststring};
        temp >> cost;
        getline(ss, effect, ';');
        getline(ss, attacks, ';');
        istringstream temp1{attacks};
        temp1 >> attack;
        getline(ss, defenses, ';');
        istringstream temp2{defenses};
        temp2 >> defense;
        getline(ss, usecosts, ';');
        istringstream temp3{usecosts};
        temp3 >> usecost;
        getline(ss, trigger, ';');
        if (type == "Spell") {
            shared_ptr<Card> c{new Spell(name, effect, cost, type)};
            d->push(c);
        } else if (type == "Ritual") {
            shared_ptr<Card> c{new Ritual(name, effect, cost, type, defense, usecost, trigger)};
            d->push(c);
        } else if (type == "Minion") {
            shared_ptr<Card> c{new Minion(name, cost, type, attack, defense)};
            shared_ptr<Minion> m{new Minion(name, cost, type, attack, defense)};
            d->pushMinion(m);
            d->push(c);
        } else if (type == "Act_Minion") {
            shared_ptr<Card> c{new Act_Minion(name, cost, effect, type, attack, defense, usecost)};
            shared_ptr<Minion> m{new Act_Minion(name, cost, effect, type, attack, defense, usecost)};
            d->pushMinion(m);
            d->push(c);
        } else if (type == "Tri_Minion") {
            shared_ptr<Card> c {new Tri_Minion(name, cost, type, attack, defense, effect, trigger)};
            shared_ptr<Minion> m {new Tri_Minion(name, cost, type, attack, defense, effect, trigger)};
            d->pushMinion(m);
            d->push(c);
        } else if (name == "Giant Strength" ) {
            shared_ptr<Card> c{new GiantStrength(name, cost, type, attack, defense, effect)};
            d->push(c);        
        } else if (name == "Magic Fatigue") {
            shared_ptr<Card> c{new MagicFatigue(name, cost, type, attack, defense, effect)};
            d->push(c);
        } else if (name == "Silence") {
            shared_ptr<Card> c{new Silence(name, cost, type, attack, defense, effect)};
            d->push(c);
        } else {
            break;
        }
    }
    f.close();
    this->get1()->setDatabase(d);
    this->get2()->setDatabase(d);
}

void Game::shuffle() {
    this->get1()->getDeck()->shuffle(1);
    this->get2()->getDeck()->shuffle(2);
}
    
// Actions
void Game::play(int i) {
    curplayer->play(i);
}
    
void Game::play(int i, int p, string t) {
    curplayer->play(i,p,t);
}

void Game::testing_play(int i) {
    curplayer->testing_play(i);
}
    
void Game::testing_play(int i, int p, string t) {
    curplayer->testing_play(i,p,t);
}
    
void Game::use(int i) {
    curplayer->use(i);
}
    
void Game::use(int i, int p, string t) {
    curplayer->use(i,p,t);
}

void Game::attack(int i) {
    curplayer->attack(i);
}
    
void Game::attack(int i, int j) {
    curplayer->attack(i,j);
}    


// Command 
void Game::inspect(int i) {
    curplayer->inspect(i);
}

//void Game::discard(int i) {
//    curplayer->discard(i);
//}
    

// Trigger Notification
void Game::notify(string t, shared_ptr<Minion> m) {
    if (t == "Enter") {
        this->curplayer->notify("MEnter", m);
        this->noncurplayer->notify("OEnter", m);
    }
    if (t == "Start" || t == "End") {
        this->curplayer->notify(t);
    }
}

void Game::notifyRitual(string t, shared_ptr<Ritual> r) {
    this->curplayer->notifyRitual("REnter", r);
    this->noncurplayer->notifyRitual("REnter", r);
}
    
// Print
// When player enters hand on the command line
void Game::handDisplay() {
    curplayer->handDisplay();
}


// When player enters baord on the command line
void Game::boardDisplay() {
    // print player 1
    player1->boardDisplay(1);
    // print center graphic
    for (auto it = CENTRE_GRAPHIC.begin() ; it != CENTRE_GRAPHIC.end(); ++it) {
        cout << *it;
        std::cout << '\n';
    }  
    // print player 2
    player2->boardDisplay(2);
}







