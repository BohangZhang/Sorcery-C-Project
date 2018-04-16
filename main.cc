#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
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
#include <map>

using namespace std;

int main(int argc, char *argv[]) {
    // ......
    bool testing_mode = 0;
    bool init_mode = 0;
    ifstream deck1{"default.deck"};
    ifstream deck2{"default.deck"};
    ifstream file;
    for (int i = 1; i < argc; ++i) {
        string command = string(argv[i]);
        if (command == "-deck1") {
            ++i;
            if (i == argc) {
                cout << "No provided deck for player 1!" << endl;
                break;
            }
            deck1.close();
            command = string(argv[i]);
            deck1.open(command);
            if (deck1.fail()) {
                cout << "Load deck for player 1 failed!" << endl;
                --i;
                deck1.clear();
            }
        } else if (command == "-deck2") {
            ++i;
            if (i == argc) {
                cout << "No provided deck for player 2!" << endl;
                break;
            }
            deck2.close();
            command = string(argv[i]);
            deck2.open(command);
            if (deck2.fail()) {
                cout << "Load deck for player 2 failed!" << endl;
                --i;
                deck2.clear();
            }
        } else if (command == "-init") {
            ++i;
            if (i == argc) {
                cout << "No provided file for input!" << endl;
                break;
            }
            command = string(argv[i]);
            file.open(command);
            init_mode = 1;
            if (file.fail()) {
                cout << "Load input file failed!" << endl;
                --i;
                file.clear();
                init_mode = 0;
            }
        } else if (command == "-testing") {
            testing_mode = 1;
        } else {
            cout << "Invalid command line argument!" << endl;
        }
    }
    
    string player1, player2;
    cout << "Enter Player1's name: " << endl;
    if (init_mode == 1) {
        getline(file, player1);
        if (file.fail()) {
            file.close();
            getline(cin, player1);
        }
    } else {
        getline(cin, player1);
    }
    // Since the game is played on the terminal and the player don't know what they have in the first, we need to show the informations here
    // e.g. "Welcome to the game, now you have 4 cards, 20 HP and 3 magic to use"
    cout << "Enter Player2's name: " << endl;
    if (init_mode == 1) {
        getline(file, player2);
        if (file.fail()) {
            file.close();
            getline(cin, player2);
        }
    } else {
        getline(cin, player2);
    }
    // check if it's -init file, if so open the file and read in input
    // check if it's -testing, if so implement testing mode
    shared_ptr<Player> p1 {new Player(player1)};
    if (testing_mode == 1) {p1->setMagic();}
    shared_ptr<Player> p2 {new Player(player2)};
    if (testing_mode == 1) {p2->setMagic();}
    shared_ptr<Game> g {new Game(p1, p2)};
    g->loadDatabase();
    
    g->loadDecks(deck1, deck2);
    deck1.close();
    deck2.close();
    if (testing_mode == 0) {
        g->shuffle();
    }
    g->get1()->setGame(g);
    g->get2()->setGame(g);
    
    g->addCard(1);
    g->addCard(1);
    g->addCard(1);
    g->addCard(1);
    g->addCard(2);
    g->addCard(2);
    g->addCard(2);
    g->addCard(2);
    
    string cmd;
    string cmdname;
    int cmdi;
    int cmdp;
    string cmdt;
    int cmdj;
    string cmdremain;
    vector<shared_ptr<Minion>> attackcount;
    
    while (1) {
        // if (g->getEnd() == true) break;
        // cout << g->getCurrentPlayer()->getName() << ": ";
        // ......
        if (g->getRound() % 2 == 0) {
            cout << "Player 2:" << endl;
        } else {
            cout << "Player 1:" << endl;
        }
        if (init_mode == 1) {
            getline(file, cmd);
            if (file.fail()) {
                file.close();
                getline(cin, cmd);
            }
        } else {
            getline(cin, cmd);
        }
        istringstream ss{cmd};
        ss >> cmdname;
        if (cmdname == "help") {
            cout << setw(10) << "Commands: " << "help -- Display this message." << endl;
            cout << setw(10) << "" << "end -- End the current player's turn." << endl;
            cout << setw(10) << "" << "quit -- End the game." << endl;
            cout << setw(10) << "" << "attack minion other-minion -- Orders minion to attack other-minion." << endl;
            cout << setw(10) << "" << "attack minion -- Orders minion to attack the opponent." << endl;
            cout << setw(10) << "" << "play card [target-player target-card] -- Play card, optionally targeting target-card owned by target-player." << endl;
            cout << setw(10) << "" << "use minion [target-player target-card] -- Use minion's special ability, optionally targeting target-card owned by target-player." << endl;
            cout << setw(10) << "" << "inspect minion -- View a minion's card and all enchantments on that minion." << endl;
            cout << setw(10) << "" << "hand -- Describe all cards in your hand." << endl;
            cout << setw(10) << "" << "board -- Describe all cards on the board." << endl;
        }
        else if (cmdname == "end") {
            for (int i = 0; i < 167; ++i) {
                cout << "*";
            }
            cout << endl << endl;
            try {
                g->nextTurn();
            } catch (int &e) {
                if (e == 1) {
                    cerr << "The activated minion has been silented." << endl;
                } else if (e == 2) {
                    cerr << "The minion slot is full." << endl;
                } else if (e == 3) {
                    cerr << "The target minion does not exist." << endl;
                } else if (e == 4) {
                    cerr << "The deck is empty." << endl;
                } else if (e == 5) {
                    cerr << "The hand is empty." << endl;
                } else if (e == 6) {
                    cerr << "You don't have enough magic." << endl;
                } else if (e == 7) {
                    cerr << "Your card does not exist." << endl;
                } else if (e == 8) {
                    cerr << "Wrong command to play enchantment." << endl;
                } else if (e == 9) {
                    cerr << "The enchantment cannot be played on ritual." << endl;
                } else if (e == 10) {
                    cerr << "Wrong command to play the card." << endl;
                } else if (e == 11) {
                    cerr << "Your minion does not exist." << endl;
                } else if (e == 12) {
                    cerr << "The minion does not have ability to use." << endl;
                } else if (e == 13) {
                    cerr << "Your hand is full." << endl;
                } else if (e == 14) {
                    cerr << "Wrong pop out from slot." << endl;
                } else if (e == 15) {
                    cerr << "You haven't play a ritual card." << endl;
                } else if (e == 16) {
                    cerr << "Your graveyard is empty." << endl;
                } else if (e == 17) {
                    cerr << "The spell cannot be played on a ritual." << endl;
                } else if (e == 18) {
                    cerr << "The target minion hasn't been enchanted." << endl;
                } else if (e == 19) {
                    cerr << "Each minion can only action one time." << endl;
                }  else {
                    cerr << "Wrong command" << endl;
                }
            }
            
            attackcount.clear();
            cout << endl;
        }
        else if (cmdname == "quit") {
            cout << "Game end." << endl;
            break;
        }
        else if (cmdname == "attack") {
            if (!(ss >> cmdi)) {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            }
            if (!(ss >> cmdj)) {
                try {
                    if (cmdi > g->getCurrent()->getSlot()->getSlot().size()) {
                        throw 11;
                    } else {
                        for (auto ele : attackcount) {
                            if (ele == g->getCurrent()->getSlot()->getSlot()[cmdi-1]) {
                                if (ele->getName() == g->getCurrent()->getSlot()->getSlot()[cmdi-1]->getName()){
                                    throw 19;
                                }
                                
                            }
                        }
                        attackcount.emplace_back(g->getCurrent()->getSlot()->getSlot()[cmdi-1]);
                        g->attack(cmdi);
                        if (g->getNoncurrent()->getHp() == 0) {
                            cout << "You Win!" << endl;
                            break;
                        }
                    }
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
                continue;
            }
            if (!(ss >> cmdremain)) {
                try {
                    if (cmdi > g->getCurrent()->getSlot()->getSlot().size()) {
                        throw 11;
                    } else {
                        for (auto ele : attackcount) {
                            if (ele == g->getCurrent()->getSlot()->getSlot()[cmdi-1]) {
                                throw 19;
                            }
                        }
                        attackcount.emplace_back(g->getCurrent()->getSlot()->getSlot()[cmdi-1]);
                        g->attack(cmdi, cmdj);
                    }
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
                continue;
            } else {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            }
        }
        else if (cmdname == "play") {
            if (!(ss >> cmdi)) {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            }
            if (!(ss >> cmdp)) {
                try {
                    if (testing_mode == 1) {
                        g->testing_play(cmdi);
                    } else {
                        g->play(cmdi);
                    }
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
                continue;
            }
            if (!(ss >> cmdt)) {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            } 
            if (!(ss >> cmdremain)) {
                try {
                    if (testing_mode == 1) {
                        g->testing_play(cmdi, cmdp, cmdt);
                    } else {
                        g->play(cmdi, cmdp, cmdt);
                    }
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
                continue;
            } else {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            }
        } 
        else if (cmdname == "use") {
            if (!(ss >> cmdi)) {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            }
            if (!(ss >> cmdp)) {
                try {
                    if (cmdi > g->getCurrent()->getSlot()->getSlot().size()) {
                        throw 11;
                    } else {
                        for (auto ele : attackcount) {
                            if (ele == g->getCurrent()->getSlot()->getSlot()[cmdi-1]) {
                                throw 19;
                            }
                        }
                        attackcount.emplace_back(g->getCurrent()->getSlot()->getSlot()[cmdi-1]);
                        g->use(cmdi);
                    }
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
                continue;
            }
            if (!(ss >> cmdt)) {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            } 
            if (!(ss >> cmdremain)) {
                try {
                    if (cmdi > g->getCurrent()->getSlot()->getSlot().size()) {
                        throw 11;
                    } else {
                        for (auto ele : attackcount) {
                            if (ele == g->getCurrent()->getSlot()->getSlot()[cmdi-1]) {
                                throw 19;
                            }
                        }
                        attackcount.emplace_back(g->getCurrent()->getSlot()->getSlot()[cmdi-1]);
                        g->use(cmdi, cmdp, cmdt);
                    }
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
                continue;
            } else {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            }
        } 
        else if (cmdname == "inspect") {
            if (!(ss >> cmdi)) {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            }
            if (!(ss >> cmdremain)) {
                try {
                    g->inspect(cmdi);
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
                continue;
            } else {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            }
        }
        else if (cmdname == "hand") {
            if (!(ss >> cmdremain)) {
                try {
                    g->handDisplay();
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
            }
        } 
        else if (cmdname == "board") {
            if (!(ss >> cmdremain)) { 
                try {
                    g->boardDisplay();
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
            }
        }
        else if (cmdname == "discard" && testing_mode == 1) {
            if (!(ss >> cmdi)) {
                cerr << "Invalid command, please see the help page!" << endl;
                continue;
            }
            if (!(ss >> cmdremain)) {
                try {
                    g->getCurrent()->getHand()->pop(cmdi);
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
                continue;
            }
        }
        else if (cmdname == "draw" && testing_mode == 1) {
            if (!(ss >> cmdremain)) {
                try {
                    g->getCurrent()->draw();
                } catch (int &e) {
                    if (e == 1) {
                        cerr << "The activated minion has been silented." << endl;
                    } else if (e == 2) {
                        cerr << "The minion slot is full." << endl;
                    } else if (e == 3) {
                        cerr << "The target minion does not exist." << endl;
                    } else if (e == 4) {
                        cerr << "The deck is empty." << endl;
                    } else if (e == 5) {
                        cerr << "The hand is empty." << endl;
                    } else if (e == 6) {
                        cerr << "You don't have enough magic." << endl;
                    } else if (e == 7) {
                        cerr << "Your card does not exist." << endl;
                    } else if (e == 8) {
                        cerr << "Wrong command to play enchantment." << endl;
                    } else if (e == 9) {
                        cerr << "The enchantment cannot be played on ritual." << endl;
                    } else if (e == 10) {
                        cerr << "Wrong command to play the card." << endl;
                    } else if (e == 11) {
                        cerr << "Your minion does not exist." << endl;
                    } else if (e == 12) {
                        cerr << "The minion does not have ability to use." << endl;
                    } else if (e == 13) {
                        cerr << "Your hand is full." << endl;
                    } else if (e == 14) {
                        cerr << "Wrong pop out from slot." << endl;
                    } else if (e == 15) {
                        cerr << "You haven't play a ritual card." << endl;
                    } else if (e == 16) {
                        cerr << "Your graveyard is empty." << endl;
                    } else if (e == 17) {
                        cerr << "The spell cannot be played on a ritual." << endl;
                    } else if (e == 18) {
                        cerr << "The target minion hasn't been enchanted." << endl;
                    } else if (e == 19) {
                        cerr << "Each minion can only action one time." << endl;
                    } else {
                        cerr << "Wrong command" << endl;
                    }
                    continue;
                }
            }
        } else {
            cerr << "Wrong command." << endl;
        }
    }
    g->getCurrent()->setDeck();
    g->getCurrent()->setHand();
    g->getCurrent()->setRitual();
    g->getCurrent()->setGraveyard();
    g->getCurrent()->setSlot();
    g->getNoncurrent()->setDeck();
    g->getNoncurrent()->setHand();
    g->getNoncurrent()->setRitual();
    g->getNoncurrent()->setGraveyard();
    g->getNoncurrent()->setSlot();
    g->getCurrent()->setGame(nullptr);
    g->getNoncurrent()->setGame(nullptr);
}
