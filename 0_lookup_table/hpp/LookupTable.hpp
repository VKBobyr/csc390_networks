#ifndef LOOKUP_HPP
#define LOOKUP_HPP

#include <iostream>
#include <set>
#include "List.hpp"

class Item;

class LookupTable {
   private:
    List *tableArray;  // maximum of 10 slots

   public:
    LookupTable();   // constructor
    ~LookupTable();  // destructor
    static const int MAX_SLOTS = 30;

    Item retrieve(std::string key);
    bool insert(std::string key, Item value);
    bool remove(std::string key);  // removes one instance of the key
    int numberUnused();            // returns number unused positions in lookupTable
    int numberUsed();              // returns number used positions in lookupTable
    int minimumCollisions();       // returns smallest number of collisions in any used lookupTable
    int maximumCollisions();       // returns largest number of collisions in any used lookupTable position
    void display();                // displays the contents of the table at each position, plus table statistics (numberUnused, numberUsed, minimumCollisiosn, maximumCollisions)
};

#endif  // LOOKUP_HPP
