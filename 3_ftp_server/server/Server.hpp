#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include "vector"
#include <filesystem>

#include "Algs.hpp"
#include "Communicator.hpp"

using namespace std;

class Server  // Keeps track of the items whose keys end up at the same table position
{
   public:
    Server();
    ~Server();
    void establishConnection();
    bool isAuthenticated();
    void commandLoop();
    Communicator api{97, 83, 61, 2581};

    void cUser(vector<string> tokens);
    void cPassword(vector<string> tokens);
    void cDir();
    void cCd(vector<string> tokens);
    void cDel(vector<string> tokens);
    void cGet();
    void cPut();

    const string savedUser = "admin";
    const string savedPassword = "pass";

    string currentUser;
    string currentPassword;
};

#endif  // SERVER_HPP
