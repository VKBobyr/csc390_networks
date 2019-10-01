#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include "Algs.hpp"
#include "Data.hpp"

using namespace std;

class Communicator {
   private:
    int rsa_their_n, rsa_their_e;                    // target's public key
    int rsa_our_p, rsa_our_q, rsa_our_d, rsa_our_e;  // our private key

    pair<char*, int> encrypt(char* target, int size) {
        int* encrypted = new int[size];
        for (int i = 0; i < size; i++) {
            encrypted[i] = Algs::fastExp((int)target[i], rsa_their_e, rsa_their_n);
        }

        int outSize = size * sizeof(int);
        char* chars = new char[outSize];
        for (int i = 0; i < size; i++) {
            memcpy(&chars[i * 4], &encrypted[i], sizeof(int));
        }
        delete[] encrypted;
        return pair<char*, int>{chars, outSize};
    }

    pair<char*, int> decrypt(char* target, int size) {
        int outSize = size / sizeof(int);
        char* decrypted = new char[outSize];
        for (int i = 0; i < outSize; i++) {
            int secret;
            memcpy(&secret, &target[i * 4], sizeof(int));
            decrypted[i] = (char)Algs::fastExp(secret, rsa_our_d, rsa_our_p * rsa_our_q);
        }
        delete[] target;
        return pair<char*, int>{decrypted, outSize};
    }

    int receiveInt() {
        char cInt[sizeof(int)];
        recv(socket, cInt, sizeof(int), 0);
        return charToInt(cInt);
    }

    void sendInt(int inty) {
        char* cInty = intToChar(inty);
        // ::send(socket, cInty, sizeof(int), 0);
        delete[] cInty;
    }

    void sendChar(char* data, int size) {
        if (rsa_their_n && rsa_their_e) {
            auto encrypted = encrypt(data, size);
            data = encrypted.first;
            size = encrypted.second;
        }

        sendInt(size);
        ::send(socket, data, size, 0);
    }

    pair<char*, int> receiveChar() {
        int totalSize = receiveInt();
        // int numRequests = receiveInt();
        char* data = new char[totalSize];

        int bytesReceived = 0;
        while (bytesReceived < totalSize) {
            bytesReceived += ::recv(socket, &data[bytesReceived], totalSize - bytesReceived, 0);
        }

        if (rsa_their_e && rsa_their_n) {
            auto decrypted = decrypt(data, totalSize);
            data = decrypted.first;
            totalSize = decrypted.second;
        }

        return pair<char*, int>(data, totalSize);
    }

    string receiveString() {
        auto cString = receiveChar();
        auto sString = string(cString.first, cString.first + cString.second);
        delete[] cString.first;
        return sString;
    }

    void sendString(string str) {
        sendInt(str.length());
        sendChar((char*)str.c_str(), str.length());
    }

   public:
    Communicator(int p, int q, int e, int d) {
        rsa_our_p = p;
        rsa_our_q = q;
        rsa_our_d = d;
        rsa_our_e = e;
    }

    ~Communicator() {
        ::close(socket);
    }

    void sendPublicKeys() {
        sendInt(rsa_our_p * rsa_our_q);
        sendInt(rsa_our_e);
    }

    void getPublicKeys() {
        rsa_their_n = receiveInt();
        rsa_their_e = receiveInt();
    }

    void setOthersPublicKey(int e, int n) {
        rsa_their_e = e;
        rsa_their_n = n;
    }

    void send(char* data = nullptr, int size = 0, int status = 100) {
        // send status
        sendInt(status);

        // send data
        sendChar(data, size);
    }

    void sendMessage(string message, int status = 100) {
        send((char*)message.c_str(), message.length(), status);
    }

    void sendStatus(int status) {
        send(nullptr, 0, status);
    }

    Data receive() {
        // receive status
        int status = receiveInt();

        // receive the data
        auto data = receiveChar();

        if (status == 400) {
            cout << "Server error: " << string(data.first, data.first + data.second) << endl;
        }

        return Data(data.first, data.second, status);
    }

    int socket;
    static const int PORT = 15001;

    static char* intToChar(int& a) {
        char* out = new char[sizeof(int)];
        memcpy(out, &a, sizeof(int));
        // delete[] out;
        return out;
    }

    static int charToInt(char* a) {
        int out;
        memcpy(&out, a, sizeof(int));
        return out;
    }
};

#endif  // COMMUNICATOR_HPP
