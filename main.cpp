#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "Database.h"
using namespace std;

int main() {
    Casher::KeyValStore db(10);
    cout<<"======================================"<<endl;
    cout<<"==========CASHER V1 TERMINAL=========="<<endl;
    cout<<"======================================"<<endl;
    cout<<"COMMANDS:"<<endl;
    cout<<"SET <key> <value> | GET <key> | DEL <key> | EXIT"<<endl;
    cout<<"Extra Commands: \n LOOPSEARCH <key> | returns the entire bucket of a provided key \n DELOLDEST | deletes the oldest LRU entry, which is tailLRU."<<endl;

    string lineinput;

    while (true) {
        cout<<"casher >";
        if (!getline(cin,lineinput)) {
            break;
        }

        stringstream commandStream(lineinput);
        string command;
        commandStream >> command;
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command.empty()) {
            continue;
        }
        if (command == "exit") {
            cout<<"Shutting down..."<<endl;
            break;
        }
        else if (command == "set") {
            string key, value;
            commandStream >> key;
            getline(commandStream>>ws, value);
            if (key.empty() || value.empty()) {
                cout<<"ERROR: key or value are empty or improperly written."<<endl;
            }
            else {
                bool success = db.set(key, value);
                if (success) {
                    std::cout << "Successfully created entry with provided key and value\n";
                } else {
                    std::cout << "ERROR: Key already exists\n";
                }
            }
        }
        else if (command == "get") {
            string key;
            commandStream >> key;
            if (key.empty()) {
                cout<<"ERROR: The key cannot be empty."<<endl;
            } else {
                string result = db.get(key);
                if (result == "nullptr") {
                    cout<<"ERROR: The key does not exist -> "<<result<<endl;
                }
                else {
                    cout<<"\""<<result<<"\""<<endl;
                }
            }
        }
        else if (command == "del") {
            string key;
            commandStream >> key;
            if (key.empty()) {
                cout<<"ERROR: The key cannot be empty."<<endl;
            } else {
                bool success = db.del(key);
                if (success) {
                    std::cout << "Successfully deleted entry with provided key\n";
                }
                else {
                    std::cout << "ERROR: Key cannot be deleted, does not exist"<<endl;
                }
            }
        }
        else if (command == "deloldest") {
            bool success = db.evictOldest();
            if (success) {
                std::cout << "Successfully deleted oldest entry\n";
            }
            else {
                std::cout << "ERROR: tail does not exist/no entries in the database"<<endl;
            }
        }
        else if (command == "loopsearch") {
            string key;
            commandStream >> key;
            if (key.empty()) {
                cout<<"ERROR: The key cannot be empty."<<endl;
            } else {
                cout <<"The bucket in question starting from the provided key is: "<<db.loopSearch(key)<<"\n";
            }
        }
        else {
            cout<<"Unknown command"<<endl;
        }

    }

    return 0;
}
