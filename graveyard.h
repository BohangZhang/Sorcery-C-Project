#ifndef GRAVYARD_H
#define GRAVYARD_H

#include <vector>
#include <memory>
//#include "minion.h"

class Minion;
class Graveyard {
    
    std::vector<std::shared_ptr<Minion>> theGrave;

    public:

    // Big 5 and Ctor
    //Graveyard(std::vector<std::shared_ptr<Minion>>);
    ~Graveyard();
    
    // Get Info
    std::vector<std::shared_ptr<Minion>> getGrave() ;
    std::shared_ptr<Minion> top() ;
    
    // Check Conditions
    bool isEmpty() ;
    
    // Push and Pop
    void pushfirst(std::shared_ptr<Minion> );
    void popfirst();
    
};

#endif
