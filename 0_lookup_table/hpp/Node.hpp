#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include "Item.hpp"

struct Node {
   public:
    Node(std::string& key, Item& data) {
        this->data = new Item(data);
        this->key = new std::string(key);
    }

    ~Node() {
        delete data;
        delete key;
    }

    Item* data=nullptr;
    std::string* key=nullptr;
    Node* next = nullptr;
    Node* prev = nullptr;
};

#endif  // NODE_HPP
