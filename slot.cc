#include <iostream>
#include "slot.h"
#include "minion.h"
using namespace std;

// Dtor
Slot::~Slot() {};

// Get Info
vector<shared_ptr<Minion>> & Slot::getSlot() {
    return this->theSlot;
}

int Slot::getSize()  {
    return theSlot.size();
}
    
// Check Conditions
bool Slot::isFull()  {
    return this->getSize() == 5;
}
    
bool Slot::isEmpty()  {
    return this->getSize() == 0;
}
    
// Push and Pop
void Slot::push(shared_ptr<Minion> m) {
    theSlot.emplace_back(m);
}

void Slot::pop(shared_ptr<Minion> m) {
    for (auto it = this->theSlot.begin(); it != this->theSlot.end(); ++it) {
        if (*it == m) {
            this->theSlot.erase(it);
            return;
        }
    }
    throw 14; // Number, Error when pop out Minion from Slot
}

