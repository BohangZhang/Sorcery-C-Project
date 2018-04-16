#ifndef RITUAL_H
#define RITUAL_H

#include <memory>
#include <string>
#include "card.h"

class Player;
class Ritual : public Card {
    
    int charge;
    int chargecost;
    std::string trigger;
    
    public:

    // Constructor and Destructor
    Ritual(std::string name, std::string effect, int cost, std::string type, int charge, int chargecost, std::string trigger);
    ~Ritual();
    
    // Get Info
    int getCharge() override;
    int getChargeCost() override;
    std::string getTrigger();
    
    // Modify Fields
    void addCharge(int);
    void subCharge(int);
    
    // Check Conditions
    bool isUsable();
    
    // Actions
    void action();
    void action(std::shared_ptr<Minion>);
    //void discard();
    
    std::shared_ptr<Card> clone() override;

    // Print
    void print() override;
};

#endif
