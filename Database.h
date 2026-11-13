//
// Created by mario on 8.11.26.
//

#ifndef CASHER_DATABASE_H
#define CASHER_DATABASE_H

#include <string>
#include <memory>

namespace Casher {
    struct Node {
        std::string key;
        std::string value;

        std::shared_ptr<Node> next;

        std::shared_ptr<Node> nextLRU;
        std::weak_ptr<Node> prevLRU;
    };
    class KeyValStore {
    private:
        static const int TABLE_SIZE = 1024;
        std::shared_ptr<Node> hashTable[TABLE_SIZE];

        std::shared_ptr<Node> headLRU;
        std::shared_ptr<Node> tailLRU;

        int maxCapacity;
        int currentCapacity;
        size_t AllocByes;

        int hashFunction(const std::string& key);
        void detachFromLRU(std::shared_ptr<Node> node);
        void moveToLRUHead(std::shared_ptr<Node> node);
    public:
        KeyValStore(int capacity);

        bool set(const std::string& key, const std::string& value);
        std::string get(const std::string& key);
        std::string loopSearch(const std::string& key);
        bool del(const std::string& key);
        bool evictOldest();

        void printDebug() const;
        size_t getMemUsage() const { return AllocByes; };
    };
}

#endif //CASHER_DATABASE_H
