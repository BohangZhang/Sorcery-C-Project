#ifndef HAND_H
#define HAND_H

#include <vector>
#include <memory>

class Card;

class Hand {
    
    std::vector<std::shared_ptr<Card>> theHand;

    public:

    // Big 5 and Ctor
    //Hand(std::vector<std::shared_ptr<Card>>);
    ~Hand();
    
    // Get Info
    std::vector<std::shared_ptr<Card>> &getHand() ;
    int getSize() ;
    
    //Check Conditions
    bool isEmpty() ;
    bool isFull() ;
    
    // Push and Pop
    void push(std::shared_ptr<Card> );
    void pop(int);
    
    // Print
    void print(); 
        
};

#endif
