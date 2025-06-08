#include <iostream>
#include <unordered_map>
using namespace std;

struct Node {
    int key, value;
    Node* prev;
    Node* next;
    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
    int capacity;
    unordered_map<int, Node*> cache;
    Node* head;
    Node* tail;

    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void insertToFront(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

public:
    LRUCache(int cap) : capacity(cap) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (cache.count(key)) {
            Node* node = cache[key];
            remove(node);
            insertToFront(node);
            return node->value;
        }
        return -1;
    }

    void put(int key, int value) {
        if (cache.count(key)) {
            Node* node = cache[key];
            node->value = value;
            remove(node);
            insertToFront(node);
        } else {
            if (cache.size() == capacity) {
                Node* lru = tail->prev;
                remove(lru);
                cache.erase(lru->key);
                delete lru;
            }
            Node* node = new Node(key, value);
            cache[key] = node;
            insertToFront(node);
        }
    }

    ~LRUCache() {
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

int main() {
    LRUCache lru(2);

    lru.put(1, 1);
    lru.put(2, 2);
    cout << "Get 1: " << lru.get(1) << endl; // returns 1
    lru.put(3, 3); // evicts key 2
    cout << "Get 2: " << lru.get(2) << endl; // returns -1 (not found)
    lru.put(4, 4); // evicts key 1
    cout << "Get 1: " << lru.get(1) << endl; // returns -1 (not found)
    cout << "Get 3: " << lru.get(3) << endl; // returns 3
    cout << "Get 4: " << lru.get(4) << endl; // returns 4

    return 0;
}
