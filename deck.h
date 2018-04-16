#ifndef DECK_H
#define DECK_H

#include <vector>
#include <memory>
#include <algorithm> 
class Card;

class Deck {
    
    std::vector<std::shared_ptr<Card>> theDeck;

    public:

    // Big 5 and Ctor
    
    virtual ~Deck();
    
    // Get Info
    std::vector<std::shared_ptr<Card>> &getDeck() ;
    
    // Check Conditions
    bool isEmpty() ;
    
    // Push and Pop
    void push(std::shared_ptr<Card>);
    void popfirst();
    
    // Shuffle
    void shuffle(int);
    
};

#endif
