#include "LookupTable.hpp"

int hash(std::string &key) {
    int position = 0;
    for (int i = 0; i < key.size(); i++) {
        position = (position * 13 + key[i]) % LookupTable::MAX_SLOTS;
    }
    return position;
}

bool findKey(std::string &key, List &table) {
    table.first();
    for (int _ = 0; _ < table.count; _++) {
        if (table.examineKey() == key) {
            return true;
        }

        if (_ < table.count - 1) {
            table.next();
        }
    }
    return false;
}

LookupTable::LookupTable() {
    tableArray = new List[MAX_SLOTS];
    for (int i = 0; i < MAX_SLOTS; i++) {
        tableArray[i] = List();
    }
}

LookupTable::~LookupTable() {
    delete []tableArray;
}

Item LookupTable::retrieve(std::string key) {
    int position = hash(key);
    auto &table = tableArray[position];

    if (findKey(key, table)) {
        return table.examineItem();
    } else {
        throw std::logic_error("Could not find key.");
    }
}

bool LookupTable::insert(std::string key, Item value) {
    int position = hash(key);
    auto &table = tableArray[position];

    if (findKey(key, table)) {
        auto i = table.examineItem();
        table.remove();
        i.count++;
        table.insertAfter(key, i);
    } else {
        table.insertAfter(key, value);
    }
    return true;
}

bool LookupTable::remove(std::string key) {
    int position = hash(key);
    auto &table = tableArray[position];

    if (findKey(key, table)) {
        auto i = table.examineItem();
        table.remove();

        if (i.count > 1) {
            i.count--;
            table.insertAfter(key, i);
        }
    } else {
        throw std::logic_error("Could not find key");
    }
    return true;
}

int LookupTable::numberUnused() {
    int sum = 0;
    for (int i = 0; i < MAX_SLOTS; i++) {
        sum += tableArray[i].count == 0;
    }
    return sum;
}

int LookupTable::numberUsed() {
    int sum = 0;
    for (int i = 0; i < MAX_SLOTS; i++) {
        sum += tableArray[i].count != 0;
    }
    return sum;
}

int LookupTable::minimumCollisions() {
    int min = 99999999;
    for (int i = 0; i < MAX_SLOTS; i++) {
        int collisions = tableArray[i].count;
        if (collisions > 0 && collisions < min) {
            min = collisions;
        }
    }
    return min;
}

int LookupTable::maximumCollisions() {
    int max = -99999999;
    for (int i = 0; i < MAX_SLOTS; i++) {
        int collisions = tableArray[i].count;
        if (collisions > 0 && collisions > max) {
            max = collisions;
        }
    }
    return max;
}

void LookupTable::display() {
    printf("\n=============\nunused: %d | used: %d | min_col: %d | max_col: %d\n",
           numberUnused(), numberUsed(), minimumCollisions(), maximumCollisions());

    for (int i = 0; i < MAX_SLOTS; i++) {
        auto &t = tableArray[i];
        t.first();
        std::cout << "table: " << i << std::endl;

        for (int _ = 0; _ < t.count; _++) {
            std::string key = t.examineKey();
            Item item = t.examineItem();

            printf("    -pos: %d | key: %-14s | count: %2d | consonants: %2d | vowels: %2d\n",
                   _, key.c_str(), item.count, item.consonants, item.vowels);

            if (_ < t.count - 1) {
                t.next();
            }
        }
    }

    std::cout << "=============\n";
}
