#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>

class Minion;
class Deck;
class Hand;
class Slot;
class Graveyard;
class Ritual;
class Game;
class Database;


class Player {
    
    std::string name;
    int hp;
    int magic;
    std::shared_ptr<Deck> deck;
    std::shared_ptr<Hand> hand;
    std::shared_ptr<Slot> slot;
    std::shared_ptr<Graveyard> graveyard;
    std::shared_ptr<Ritual> ritual;
    std::shared_ptr<Database> database;
    std::shared_ptr<Game> game;
    
    public:
    
    // Big 5
    Player(std::string name);
    ~Player();
    
    // Get Info
    std::string getName() ;
    int getHp() ;
    int getMagic() ;
    std::shared_ptr<Ritual> getRitual() ;
    std::shared_ptr<Game> getGame() ;
    std::shared_ptr<Slot> getSlot() ;
    std::shared_ptr<Graveyard> getGraveyard() ;
    std::shared_ptr<Deck> getDeck() ;
    std::shared_ptr<Database> getDatabase() ;
    std::shared_ptr<Hand> getHand() ;
    
    // Modify Fields
    void addMagic(int);
    void subMagic(int);
    void setMagic();
    void addHp(int);
    void subHp(int);
    
    // Check Conditions
    bool isHandFull() ;
    bool isHandEmpty() ;
    bool isDeckEmpty() ;
    bool isSlotFull() ;
    bool isSlotEmpty() ;
    bool isGraveyardEmpty() ;
    bool isRitualEmpty() ;
    bool isDead() ;
    
    // load Cards
    void setDatabase(std::shared_ptr<Database>);
    void setGame(std::shared_ptr<Game>);
    void setDeck();
    void setHand();
    void setSlot();
    void setGraveyard();
    void setRitual(std::shared_ptr<Ritual> r = nullptr);
    
    // Actions
    void play(int);
    void play(int, int, std::string);
    void testing_play(int);
    void testing_play(int, int, std::string); 
    void use(int);
    void use(int, int, std::string);
    void attack(int);
    void attack(int, int);
    void notify(std::string, std::shared_ptr<Minion> m = nullptr);
    void notifyRitual(std::string, std::shared_ptr<Ritual> r);
    
    // Command
    void inspect(int);
    void draw();
    void testdraw();
    void discard(int);
    
    // Print
    void handDisplay() ;
    void boardDisplay(int) ;
    void minion_slot_print();
};


#endif
