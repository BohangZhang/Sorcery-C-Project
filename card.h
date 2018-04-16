#ifndef CARD_H
#define CARD_H

#include <string>
#include <memory>


class Player;

class Card {
    
    protected:
    
    std::string name;
    std::string effect;
    int cost;
    std::string type;
    std::shared_ptr<Player> master;
    
    public:
    
    // Big 5 and Ctor
    Card(std::string name, std::string effect, int cost, std::string type);
    virtual ~Card();
    
    // Get Info
    std::string getName();
    std::string getType();
    std::string getEffect();
    int getCost();
    std::shared_ptr<Player> getMaster();
    virtual int getChargeCost();
    virtual int getCharge();
    virtual int getAttack();
    virtual int getDefense();
    virtual std::string getTrigger();
    virtual int getAbilityCost();
    
    void setMaster(std::shared_ptr<Player>);
    
    // Action
    virtual void action();
    virtual void action(int, std::string);
    
    // Clone a heap card
    virtual std::shared_ptr<Card> clone() = 0;
    
    // Print
    virtual void print() = 0;
};

#endif
