#ifndef MINION_H
#define MINION_H

#include <memory>
#include <string>
#include <iostream>
#include "card.h"
#include "ascii_graphics.h"
//#include "card.h"
//#include "player.h"

class Player;
class Ritual;

class Minion : public Card {
    
    protected:
    
    int attack;
    int defense;
    bool silent;
    
    public:

    // Big5 and Ctor
    Minion(std::string name, int cost, std::string type, int attack, int defense, std::string effect = "");
    virtual ~Minion();
    
    // Get Info
    virtual int getAttack() override;
    virtual int getDefense() override;
    virtual int getAbilityCost() override;
    virtual bool getSilent() ;
    virtual std::string getTrigger() override;
    virtual std::shared_ptr<Minion> getOrigin() ;
    virtual std::string getOriginName() ;
    virtual std::string getOriginEffect() ;
    virtual int getOriginCost() ;
    virtual std::string getOriginType() ;
    virtual std::shared_ptr<Player> getOriginMaster() ;
    virtual std::string getTriggerType() ;
    
    // Modify Fields
    virtual void addAttack(int);
    virtual void addDefense(int);
    virtual void subAttack(int);
    virtual void subDefense(int);
    
    // Check Conditions
    virtual bool ifDead();
    
    // Actions
    virtual void attackPlayer();
    virtual void attackMinion(int);
    virtual void use();
    virtual void use(int, std::string);
    virtual void action(std::shared_ptr<Minion> m = nullptr);
    virtual void actionRitual(std::shared_ptr<Ritual> r);
    virtual void setOrigin(std::shared_ptr<Minion>);
    
    std::shared_ptr<Card> clone() override;
    virtual std::shared_ptr<Minion> cloneMinion();
    
    // Print
    virtual void print() override;
};

#endif 
