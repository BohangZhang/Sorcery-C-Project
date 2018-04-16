#ifndef TRI_MINION_H
#define TRI_MINION_H

#include <memory>
#include <string>
#include "minion.h"

class Card;

class Tri_Minion: public Minion {
    
    std::string trigger;
    
    public:
    // Big 5 and Ctor
    Tri_Minion(std::string name, int cost, std::string type, int attack, int defense, std::string effect, std::string trigger);
    ~Tri_Minion();
    
    // Getter 
    std::string getTrigger() override;
    
    // Actions
    void action(std::shared_ptr<Minion> m = nullptr) override;
    void actionRitual(std::shared_ptr<Ritual> r) override;
    
    std::shared_ptr<Card> clone() override;
    std::shared_ptr<Minion> cloneMinion() override;
    
    // Print
    void print()  override;
    
};

#endif
