#ifndef LIST_HPP
#define LIST_HPP

#include "Item.hpp"
#include "Node.hpp"
#include <iostream>

class List // Keeps track of the items whose keys end up at the same table position
{
private:
    Node* current; // current node in list of Nodes

public:
    List(); // constructor
    ~List(); // destructor
    int currentPos; // current location (-1 if empty or invalid location)
    void first(); // reset list position to first item
    void last(); // reset list position to last item
    void makecurrent(int position); // reset list position to position
    void prev(); // reset list position to previous item
    void next(); // reset list position to next item
    Item examineItem(); // get item at current location
    std::string examineKey(); // get key at current location
    int count = 0; // return number of items in list
    void insertBefore(std::string key, Item value); // insert item before current position
    void insertAfter(std::string key, Item value); // insert item after current position
    void remove(); // remove current item, next node becomes current node (unless current node was last node in list in which case next node becomes previous node)
    void replace(std::string key, Item value); // replace current item with item
    bool empty(); // true if list is empty
};

#endif // LIST_HPP
