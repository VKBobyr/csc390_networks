#include <fstream>
#include "LookupTable.hpp"

Item createItem(std::string key) {
    std::set<char> vowels{'i', 'a', 'o', 'u', 'e'};
    int vow = 0;
    int con = 0;
    for (int i = 0; i < key.size(); i++) {
        if (vowels.find(key[i]) != vowels.end()) {
            vow++;
        } else {
            con++;
        }
    }
    return Item(con, vow);
}

void test() {
    LookupTable table;

    table.insert("when", createItem("when"));
    table.display();
    table.insert("can", createItem("can"));
    table.display();
    table.insert("sailing", createItem("sailing"));
    table.display();
    table.insert("weather", createItem("weather"));
    table.display();
    table.insert("weather", createItem("weather"));
    table.display();

    Item i = table.retrieve("when");
    std::cout << "count for when is: " << i.count << std::endl;  // should be 1

    i = table.retrieve("weather");
    std::cout << "count for weather is: " << i.count << std::endl;  // should be 2

    table.remove("when");
    table.remove("weather");

    i = table.retrieve("weather");
    std::cout << "count for weather is: " << i.count << std::endl;  // should  be 1

    table.display();
}

int main() {
    LookupTable table;

    std::ifstream infile("data.txt");

    std::string key;
    while (infile >> key ){
        table.insert(key, createItem(key));
    }

    table.display();

    Item i = table.retrieve("when");
    std::cout << "count for when is: " << i.count << std::endl;  // should be 1

    i = table.retrieve("weather");
    std::cout << "count for weather is: " << i.count << std::endl;  // should be 5

    table.remove("when");
    table.remove("weather");

    i = table.retrieve("weather");
    std::cout << "count for weather is: " << i.count << std::endl;  // should be 4

    table.display();
}
