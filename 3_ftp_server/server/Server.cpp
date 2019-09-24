#include "Server.hpp"

int main() {
    Server server;
}

Server::Server() {
    establishConnection();
    commandLoop();
}

Server::~Server() {
}

bool Server::isAuthenticated() {
    return currentUser == savedUser && currentPassword == savedPassword;
}

void Server::establishConnection() {
    // Create a socket.
    int sSocket = socket(PF_INET, SOCK_STREAM, 0);

    if (sSocket <= 0) {
        cerr << "server)Socket creation failed." << endl;
        exit(-1);
    }

    // Create a port to listen to for client connections
    int socketSize = sizeof(sockaddr);
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(Communicator::PORT);

    int status = ::bind(sSocket, (struct sockaddr *)&address, socketSize);

    if (status != 0) {
        cerr << "server)Bind failed, unable to create port." << endl;
        exit(-1);
    }

    // Wait for client to connect to port
    cout << "server)Waiting for client to connect" << endl;
    listen(sSocket, 3);
    int cSocket = accept(sSocket, (struct sockaddr *)&address, (socklen_t *)&socketSize);

    if (cSocket <= 0) {
        cerr << "server)Accept failed, client can't connect to port." << endl;
        cout << strerror(cSocket) << endl;
        exit(-1);
    }

    cout << "Server and client have established communication." << endl;
    api.socket = cSocket;
    cout << "Exchanging public keys" << endl;
    api.getPublicKeys();
    api.sendPublicKeys();
}

void Server::commandLoop() {
    while (true) {
        string command = api.receive().contentToString();
        vector<string> commandTokens;
        Algs::tokenize(command, commandTokens, " ", true);

        if (commandTokens.size() == 0) {
            api.sendMessage("Invalid request.", 400);
            continue;
        }

        command = commandTokens[0];

        if (command == "user")
            cUser(commandTokens);
        else if (command == "password")
            cPassword(commandTokens);
        else if (command == "quit")
            exit(1);
        else if (!isAuthenticated())
            api.sendMessage("Authentication required.", 400);
        else if (command == "dir")
            cDir();
        else if (command == "cd")
            cCd(commandTokens);
        else if (command == "del")
            cDel(commandTokens);
        else if (command == "get")
            cGet();
        else if (command == "put")
            cPut();
        else {
            api.sendMessage("Unknown command.", 400);
        }
    }
}

void Server::cUser(vector<string> commandTokens) {
    if (commandTokens.size() != 2) {
        api.sendMessage("username missing", 400);
        return;
    }
    currentUser = commandTokens.at(1);
    currentPassword = "";
    api.sendStatus(200);
}

void Server::cPassword(vector<string> commandTokens) {
    if (commandTokens.size() != 2) {
        api.sendMessage("password missing", 400);
        return;
    }

    currentPassword = commandTokens.at(1);
    if (isAuthenticated()) {
        api.sendMessage("Authenticated.", 200);
    } else {
        api.sendMessage("Invalid username or password.", 200);
    }
}

void Server::cDir() {
    api.sendMessage(Algs::exec("ls"), 200);
}

void Server::cCd(vector<string> commandTokens) {
    if (!chdir(commandTokens[1].c_str())) {
        api.sendStatus(200);
    } else {
        api.sendMessage("No such directory.", 200);
    }
}
void Server::cDel(vector<string> commandTokens) {
    api.sendMessage(Algs::exec("rm " + commandTokens[1]), 200);
}
void Server::cGet() {
    auto filename = api.receive().contentToString();
    if (!Algs::exists(filename)) {
        api.sendMessage("File doesnt exist.", 400);
        return;
    }
    auto contents = Algs::readFile(filename);
    api.send((char *)contents.c_str(), contents.length());
}

void Server::cPut() {
    string filename = api.receive().contentToString();
    if (Algs::exists(filename)) {
        api.sendMessage("Filename already exists.", 400);
        return;
    } else {
        api.sendStatus(200);
    }

    auto data = api.receive();
    Algs::writeFile(filename, data.content, data.size);
    delete data.content;
    api.sendStatus(200);
}
