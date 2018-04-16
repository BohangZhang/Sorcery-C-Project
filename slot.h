#ifndef SLOT_H
#define SLOT_H

#include <vector>
#include <memory>

class Minion;

class Slot {
    
    std::vector<std::shared_ptr<Minion>> theSlot;
    
    public:

    // Dtor
    
    ~Slot();
    
    // Get Info
    std::vector<std::shared_ptr<Minion>> & getSlot() ;
    int getSize() ;
    
    // Check Conditions
    bool isFull() ;
    bool isEmpty() ;
    
    // Push and Pop
    void push(std::shared_ptr<Minion> );
    void pop(std::shared_ptr<Minion> );
    
};


#endif

