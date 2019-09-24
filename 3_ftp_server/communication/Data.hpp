#ifndef DATA_HPP
#define DATA_HPP

#include <cstring>
#include <iostream>

class Communicator;

class Data {
   public:
    Data(char* content, int size, int status) {
        this->content = content;
        this->size = size;
        this->status = status;
    }

    ~Data() {
        // delete content;
    }

    std::pair<char*, int> toChar() {
        int outSize = size + sizeof(int);
        char* out = new char[outSize];
        int index = 0;
        std::memcpy(&out[index], &size, sizeof(int));
        index += sizeof(int);
        std::memcpy(&out[index], content, size);
        return std::pair<char*, int>(out, outSize);
    }

    int size;
    char* content;
    int status;

    std::string contentToString() {
        return std::string(content, content + size);
    }
};

#endif  // DATA_HPP
