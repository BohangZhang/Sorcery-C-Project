#ifndef SPELL_H
#define SPELL_H

#include <memory>
#include <string>
#include "card.h"
#include "ascii_graphics.h"

class Player;

class Spell : public Card {
    
    public:

    // Big 5 and Ctor
    Spell(std::string name, std::string effect, int cost, std::string type);
    ~Spell();
    
    // Actions
    void action() override;
    void action(int, std::string) override;
    
    // Trigger Notification
    //void notify(std::string);
    
    // Clone a heap object
    std::shared_ptr<Card> clone() override;
    
    // Print
    void print() override;

};
    
#endif
