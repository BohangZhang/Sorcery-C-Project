#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
//#include "player.h"
#include <memory>
#include <fstream>

class Player;
class Minion;
class Ritual;

class Game {
    
    int Round;
    bool eog;
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
    std::shared_ptr<Player> curplayer;
    std::shared_ptr<Player> noncurplayer;
    
    public:
    
    // Big 5
    Game(std::shared_ptr<Player>, std::shared_ptr<Player>);
    ~Game();
    
    // Get Info
    int getRound() ;
    std::shared_ptr<Player> getCurrent();
    std::shared_ptr<Player> getNoncurrent();
    std::shared_ptr<Player> get1();
    std::shared_ptr<Player> get2();
    
    // Modify Fields
    void addCard(int);
    void addMagic(int, int);
    
    // Conditions
    void nextTurn();
    bool gameOver();
    
    // load Cards
    void loadDecks(std::ifstream &, std::ifstream &);
    void loadDatabase();
    void shuffle();
    
    // Actions
    void play(int);
    void play(int, int, std::string);
    void testing_play(int);
    void testing_play(int, int, std::string);
    void use(int);
    void use(int, int, std::string);
    void attack(int);
    void attack(int, int);
    
    // Command 
    void inspect(int i);
    //void discard(int i);
    
    // Trigger Notification
    void notify(std::string, std::shared_ptr<Minion> m = nullptr);
    void notifyRitual(std::string, std::shared_ptr<Ritual> r);
    
    // Print
    void handDisplay();
    void boardDisplay();
    
};
#endif
