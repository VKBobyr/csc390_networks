#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <filesystem>
#include <iostream>
#include "Algs.hpp"
#include "Communicator.hpp"

class Client {
   public:
    Client(char *);

    ~Client();
    void establishConnection(char *);
    void executionLoop();
    Communicator api{79, 89, 233, 3977};

    bool isAuthenticated();
    void cHelp(std::vector<std::string> tokens);
    void cGet(std::vector<std::string> tokens);
    void cPut(std::vector<std::string> tokens);
    void cLcd(std::vector<std::string> tokens);
    void oneWayCommand(std::string);
    void exitApp();
};

#endif  // CLIENT_HPP
