#ifndef ENCHANTEMENT_H
#define ENCHANTEMENT_H

#include <memory>
#include <string>
#include <sstream>
#include "minion.h"

class Card;

class Enchantment : public Minion {
    
    protected:
    
    std::shared_ptr<Minion> origin;
    std::string astring;
    std::string dstring;
    
    public:

    // ructor and Destructor
    Enchantment(std::string name, int cost, std::string type, int attack, int defense, std::string effect);
    virtual ~Enchantment();
    
    // Get Info
    std::shared_ptr<Minion> getOrigin()  override;
    std::string getOriginName()  override;
    std::string getOriginEffect()  override;
    int getOriginCost()  override;
    std::string getOriginType()  override;
    std::shared_ptr<Player> getOriginMaster()  override;
    std::string getTriggerType()  override;

    
    // Modify Fields
    void addAttack(int) override;
    void addDefense(int) override;
    void subAttack(int) override;
    void subDefense(int) override;
    
    // Check Conditions
    bool ifDead() override;
    
    // Actions
    void use() override;
    void use(int, std::string) override;
    void attackPlayer() override;
    void attackMinion(int) override;
    void action(std::shared_ptr<Minion> m = nullptr) override;
    void setOrigin(std::shared_ptr<Minion>) override;
    
    std::shared_ptr<Card> clone() override;
    std::shared_ptr<Minion> cloneMinion() override;
    
    // Print
    void print() override;
};


// GiantStrength
class GiantStrength : public Enchantment {
    
    public:
    
    // ructor and Destructor
    GiantStrength(std::string name, int cost, std::string type, int attack, int defense, std::string effect);
    virtual ~GiantStrength();
    
    
    int getAttack() override;
    int getDefense() override;
    int getAbilityCost() override;
    bool getSilent() override;
    
    std::shared_ptr<Card> clone() override;
    std::shared_ptr<Minion> cloneMinion() override;
    
};


// MagicFatigue
class MagicFatigue : public Enchantment {
    
    public:
    
    // ructor and Destructor
    MagicFatigue(std::string name, int cost, std::string type, int attack, int defense, std::string effect);
    virtual ~MagicFatigue();
    
    int getAttack()  override;
    int getDefense()  override;
    int getAbilityCost()  override;
    bool getSilent()  override;
    
    std::shared_ptr<Card> clone() override;
    std::shared_ptr<Minion> cloneMinion() override;
    
};


// Silence
class Silence : public Enchantment {
    
    public:
    
    // ructor and Destructor
    Silence(std::string name, int cost, std::string type, int attack, int defense, std::string effect);
    virtual ~Silence();
    
    int getAttack()  override;
    int getDefense()  override;
    int getAbilityCost()  override;
    bool getSilent()  override;
    
    std::shared_ptr<Card> clone() override;
    std::shared_ptr<Minion> cloneMinion() override;
    
};
#endif
