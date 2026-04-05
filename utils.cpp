#include "utils.h"
#include <fstream>
#include <ctime>
#include <functional>
#include <iostream>
using namespace std;

void logAction(const string &message){
    ofstream file("logs.txt", ios::app);

    if(!file.is_open()){
        cout << "Failed to open log file\n";
        return;
    }

    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp = timestamp.substr(0, timestamp.length()-1);

    file << "[" << timestamp << "] " << message << endl;
}

string hashPassword(const string &password){
    hash<string> hasher;
    return to_string(hasher(password));
}
