#include "List.hpp"

void link(Node* n1, Node* n2) {
    n1->next = n2;
    n2->prev = n1;
}

List::List() {
    currentPos = -1;
    current = nullptr;
}

List::~List() {
    while (!empty()) {
        remove();
    }
}

void List::first() {
    if (current == nullptr) {
        return;
    }

    while (current->prev != nullptr) {
        prev();
    }
}

void List::last() {
    if (current == nullptr) {
        return;
    }

    while (current->next != nullptr) {
        next();
    }
}

void List::makecurrent(int position) {
    if (position >= count) {
        throw std::out_of_range("Index out of bound.");
    }

    bool forward = position > currentPos;
    while (position != currentPos) {
        if (forward) {
            next();
        } else {
            prev();
        }
    }
}

void List::prev() {
    if (current->prev == nullptr) {
        throw std::out_of_range("Tried to switch to a null node");
    }

    current = current->prev;
    // printf("switched backward to %s\n", current->key->c_str());
    currentPos--;
}

void List::next() {
    if (current->next == nullptr) {
        throw std::out_of_range("Tried to switch to a null node.");
    }

    current = current->next;
    // printf("switched forward to %s\n", current->key->c_str());
    currentPos++;
}

Item List::examineItem() {
    return *current->data;
}

std::string List::examineKey() {
    return *current->key;
}

void List::insertBefore(std::string key, Item value) {
    auto newNode = new Node(key, value);

    if (count == 0) {
        current = newNode;
    } else {
        auto prev = current->prev;
        link(newNode, current);

        if (prev != nullptr) {
            link(prev, newNode);
        }
    }
    currentPos++;
    count++;
}

void List::insertAfter(std::string key, Item value) {
    auto newNode = new Node(key, value);

    if (count == 0) {
        current = newNode;
        currentPos = 0;
    } else {
        auto next = current->next;
        link(current, newNode); // cant come from here
        if (next != nullptr) {
            link(newNode, next); // or here
        }
    }
    count++;
}

void List::remove() {
    // int *a = nullptr;
    // auto b = char(*a);

    if (empty()) {
        throw std::out_of_range("Tried to remove from an empty list.");
    }

    Node* prev = current->prev;
    Node* next = current->next;
    delete current;
    current = nullptr;
    count--;

    if (empty()) {
        currentPos = -1;
    } else if (next != nullptr) {
        current = next;

        if (prev != nullptr) {
            link(prev, current);
        } else {
            current->prev = nullptr;
        }
    } else {
        current = prev;
        current->next = nullptr;
        currentPos--;
    }
}

void List::replace(std::string key, Item value) {
    current->data = new Item(value);
    current->key = new std::string(key);
}

bool List::empty() {
    return count == 0;
}

void printAll(List* l) {
    printf("Count: %d | Currrent position: %d\n", l->count, l->currentPos);

    l->first();
    auto key = l->examineKey();
    Item item = l->examineItem();
    printf("position %d | key: %s | consonants: %d | vowels: %d | count: %d\n", l->currentPos, key.c_str(), item.consonants, item.vowels, item.count);

    while (l->currentPos < l->count - 1) {
        l->next();
        auto key = l->examineKey();
        Item item = l->examineItem();
        printf("position %d | key: %s | consonants: %d | vowels: %d | count: %d\n", l->currentPos, key.c_str(), item.consonants, item.vowels, item.count);
    }
}

// void test() {
//     List l;
//     printf("size %d\n", l.count);

//     l.insertAfter("string2", Item(3, 3));
//     l.insertAfter("string4", Item(3, 4));
//     l.insertBefore("string0", Item(2, 1));
//     l.insertAfter("string3", Item(0, 0));
//     l.insertBefore("string1", Item(4, 5));

//     printAll(&l);

//     l.makecurrent(0);
//     l.remove();

//     printAll(&l);

//     l.makecurrent(3);
//     l.remove();

//     printAll(&l);

//     l.makecurrent(1);
//     l.remove();

//     printAll(&l);

//     l.makecurrent(0);
//     l.replace("string2", Item(4, 4));
//     printAll(&l);
// }
