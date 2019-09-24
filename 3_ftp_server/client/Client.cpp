#include "Client.hpp"
using namespace std;

Client::Client(char* IP) {
    establishConnection(IP);
    executionLoop();
}

void Client::executionLoop() {
    while (true) {
        std::cout << "(ftp) $~: ";
        std::string command;
        cout.flush();
        getline(cin, command);

        if (command.length() == 0) {
            exitApp();
        }

        std::vector<std::string> commandTokens;
        Algs::tokenize(command, commandTokens, " ", true);
        auto head = commandTokens[0];
        if (head == "help")
            cHelp(commandTokens);
        else if (head == "lcd")
            cLcd(commandTokens);
        else if (head == "get")
            cGet(commandTokens);
        else if (head == "put")
            cPut(commandTokens);
        else if (head == "quit" || cin.eof())
            exitApp();
        else
            oneWayCommand(command);
    }
}

void Client::exitApp() {
    exit(1);
    api.sendMessage("quit");
}

void Client::cHelp(std::vector<std::string> commandTokens) {
    cout << "-	HELP: lists ftp commands with explanations\n"
            "-	USER: specifies a user for authentication\n"
            "-	PASSWORD: specifies a user’s password for authentication\n"
            "-	DIR: lists the files in the server’s current directory\n"
            "-	CD <path>: changes the server’s current directory to the new path.  The path can be relative or absolute\n"
            "-	LCD <path: changes the client’s current directory to the new path.  The path can be relative or absolute\n"
            "-	DEL <filename>: deletes the server file specified by <filename> in the current directory\n"
            "-	GET <filename>: gets the server file specified by <filename> in the current directory and places the file in the client directory using the same <filename>\n"
            "-	PUT <filename>: puts the client file specified by <filename> in the current directory and places the file in the server directory using the same <filename>\n"
            "-  QUIT: shut down the client and the server"
         << endl;
}

void Client::cLcd(std::vector<std::string> commandTokens) {
    if (chdir(commandTokens[1].c_str())) {
        cout << "No such directory." << endl;
    }
}

void Client::cGet(std::vector<std::string> commandTokens) {
    if (commandTokens.size() < 2) {
        cout << "Filename missing" << endl;
        return;
    }

    auto filepath = commandTokens[1];
    auto filename = filepath.substr(filepath.find_last_of("/") + 1);

    // we cant download a file with a name that already exists
    if (Algs::exists(filename)) {
        cout << "Filename already exists." << endl;
        return;
    }

    // send the get command
    api.sendMessage(commandTokens[0]);

    // send the path to file we want to get
    api.sendMessage(filepath);

    cout << "Getting... ";

    // make sure the server is okay
    auto response = api.receive();
    if (response.status == 400) {
        return;
    }

    // write the contents to a file
    Algs::writeFile(filename, response.content, response.size);
    cout << "Done." << endl;
}
void Client::cPut(std::vector<std::string> commandTokens) {
    string filepath = commandTokens[1];
    if (!Algs::exists(filepath)) {
        cout << "File doesnt exist." << endl;
        return;
    }

    // get the name
    string filename = filepath.substr(filepath.find_last_of('/') + 1);

    // send the put command
    api.sendMessage(commandTokens[0]);

    // send the file name
    api.sendMessage(filename);
    auto response = api.receive();

    // make sure server is okay with the name
    if (response.status != 200) {
        return;
    }

    // read the file
    auto contents = Algs::readFile(filepath);

    cout << "Sending... ";
    // send the file
    api.send((char*)contents.c_str(), contents.length());

    // get final confirmation
    response = api.receive();
    if (response.status != 200) {
        return;
    }
    cout << "Done." << endl;
}

void Client::oneWayCommand(std::string command) {
    api.sendMessage(command, 100);
    auto response = api.receive();
    auto msg = response.contentToString();
    if (response.status == 400) {
        return;
    } else if (msg.length() > 0) {
        cout << msg << endl;
    }
}

Client::~Client() {
    exitApp();
}

void Client::establishConnection(char* IP) {
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s <= 0) {
        cerr << "clientr: Socket creation failed." << endl;
        exit(-1);
    }

    ////////////////////////////////////////////////////////////////////
    // Connect to server port
    ////////////////////////////////////////////////////////////////////
    sockaddr_in address;
    int addressSize = sizeof(sockaddr_in);
    address.sin_family = AF_INET;
    address.sin_port = htons(Communicator::PORT);
    inet_pton(AF_INET, IP, &address.sin_addr);

    int status = 0;
    status = connect(s, (struct sockaddr*)&address, addressSize);

    if (status != 0) {
        cerr << "client:Connect failed, can't connect to port." << endl;
        cout << strerror(errno) << endl;
        exit(-1);
    }

    cout << "Server and client have established communication." << endl;
    api.socket = s;
    cout << "Exchanging public keys" << endl;
    api.sendPublicKeys();
    api.getPublicKeys();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Enter server IP as a command line argument." << endl;
        return -1;
    }
    Client client(argv[1]);
}