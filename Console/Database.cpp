//
// Created by mario on 8.11.26.
//

#include "Database.h"
#include <fstream>

namespace Casher {
    KeyValStore::KeyValStore(int capacity) {
        maxCapacity = capacity;

        currentCapacity = 0;
        AllocByes = 0;
        headLRU = nullptr;
        tailLRU = nullptr;
    }

    int KeyValStore::hashFunction(const std::string &key) {
        unsigned long hash = 5381;
        char c;
        for (int i=0; i<key.size(); i++) {
            c=key[i];
            hash = ((hash<<5)+hash)+c;
        }
        return hash % 1024;
    }

    bool KeyValStore::set(const std::string &key, const std::string &value) {
        int loc = hashFunction(key);

        auto n = std::make_shared<Node>();
        n->key = key;
        n->value = value;

        if (headLRU == nullptr) {
            hashTable[loc] = n;
            headLRU = n;
            tailLRU = n;
        }
        else {
            auto cur = hashTable[loc];
            while (cur != nullptr) {
                if (cur->key == key) {
                    return false;
                }
                cur = cur->next;
            }
            n->next = hashTable[loc];
            hashTable[loc] = n;

            n->nextLRU = headLRU;
            headLRU->prevLRU = n;
            headLRU = n;

        }

        return true;
    }
    std::string KeyValStore::get(const std::string &key) {
        int loc = hashFunction(key);
        auto cur = hashTable[loc];
        if (cur == nullptr) {
            return "nullptr";
        }
        while (cur != nullptr) {
            if (cur->key == key) {
                return cur->value;
            }
            cur = cur->next;
        }
        return "nullptr";
    }
        bool KeyValStore::del(const std::string &key) {
        int loc = hashFunction(key);
        auto cur = hashTable[loc];
        std::shared_ptr<Node> prevHash = nullptr;

        if (cur == nullptr) {
            return false;
        }

        while (cur != nullptr) {
            if (cur->key == key) {
                if (prevHash != nullptr) {
                    prevHash->next = cur->next;
                } else {
                    hashTable[loc] = cur->next;
                }

                std::shared_ptr<Node> safePrev = cur->prevLRU.lock();

                if (cur->nextLRU != nullptr) {
                    cur->nextLRU->prevLRU = cur->prevLRU;
                } else {
                    if (tailLRU == cur) {
                        tailLRU = safePrev;
                    }
                }

                if (safePrev != nullptr) {
                    safePrev->nextLRU = cur->nextLRU;
                } else {
                    if (headLRU == cur) {
                        headLRU = cur->nextLRU;
                    }
                }

                return true;
            }

            prevHash = cur;
            cur = cur->next;
        }
        return false;
    }
    bool KeyValStore::evictOldest() {
        if (tailLRU != nullptr) {
            del(tailLRU->key);
            return true;
        }
        return false;
    }
    std::string KeyValStore::loopSearch(const std::string &key) {
        std::string res = "";
        int loc = hashFunction(key);
        auto cur = hashTable[loc];
        while (cur != nullptr) {
            res += cur->value + " -> ";
            cur = cur->next;
        }
        res += "nullptr";
        return res;
    }

    void KeyValStore::saveToFile(const char *filename) {
        std::ofstream out(filename);
        if (!out) return;

        auto current = tailLRU;
        while (current != nullptr) {
            out<<current->key<<"\t"<<current->value<<"\n"<<std::endl;
            current = current->prevLRU.lock();
        }
    }

    void KeyValStore::loadFromFile(const char *filename) {
        std::ifstream in(filename);
        if (!in) return;

        std::string line;
        while (std::getline(in, line)) {
            size_t tabPos = line.find("\t");
            if (tabPos != std::string::npos) {
                std::string key = line.substr(0, tabPos);
                std::string value = line.substr(tabPos+1);

                set(key, value);
            }
        }
    }
}

