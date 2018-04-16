#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <string>
#include <memory>


class Card;
class Minion;
class Database {
    
    std::map<std::string, std::shared_ptr<Card>> allcards;
    std::map<std::string, std::shared_ptr<Minion>> allminions;

    public:
    
    // Dtor
    ~Database();
    
    std::shared_ptr<Card> getCard(std::string);
    std::shared_ptr<Minion> getMinion(std::string);
    void push(std::shared_ptr<Card>);
    void pushMinion(std::shared_ptr<Minion>);
    
};

#endif 
