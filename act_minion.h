#ifndef ACT_MINION_H
#define ACT_MINION_H

#include <memory>
#include <string>
#include "minion.h"

class Card;

class Act_Minion : public Minion {
    
    int abilityCost;
    
    public :
    // Big 5 and Ctor
    Act_Minion(std::string name, int cost, std::string effect, std::string type, int attack, int defense, int abilityCost);
    virtual ~Act_Minion();
    
    // Getter
    int getAbilityCost() override;
    
    // Actions
    void use() override;
    void use(int, std::string) override;
    
    std::shared_ptr<Card> clone() override;
    std::shared_ptr<Minion> cloneMinion() override;
    
    // Print
    void print() override;

};
#endif
