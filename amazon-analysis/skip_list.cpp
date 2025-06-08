
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <climits>
using namespace std;

class Node {
public:
    int key;
    vector<Node*> forward;

    Node(int key, int level) : key(key), forward(level + 1, nullptr) {}
};

class SkipList {
    int MAX_LEVEL;
    float P;
    int level;
    Node* header;

public:
    SkipList(int maxLevel = 16, float probability = 0.5)
        : MAX_LEVEL(maxLevel), P(probability), level(0) {
        header = new Node(INT_MIN, MAX_LEVEL);
        srand((unsigned)time(0));
    }

    int randomLevel() {
        int lvl = 0;
        while (((float)rand() / RAND_MAX) < P && lvl < MAX_LEVEL)
            lvl++;
        return lvl;
    }

    void insert(int key) {
        vector<Node*> update(MAX_LEVEL + 1);
        Node* x = header;
        for (int i = level; i >= 0; i--) {
            while (x->forward[i] && x->forward[i]->key < key)
                x = x->forward[i];
            update[i] = x;
        }
        x = x->forward[0];

        if (!x || x->key != key) {
            int lvl = randomLevel();
            if (lvl > level) {
                for (int i = level + 1; i <= lvl; i++)
                    update[i] = header;
                level = lvl;
            }
            Node* newNode = new Node(key, lvl);
            for (int i = 0; i <= lvl; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
            cout << "Inserted key: " << key << endl;
        }
    }

    bool search(int key) {
        Node* x = header;
        for (int i = level; i >= 0; i--) {
            while (x->forward[i] && x->forward[i]->key < key)
                x = x->forward[i];
        }
        x = x->forward[0];
        return x && x->key == key;
    }

    void erase(int key) {
        vector<Node*> update(MAX_LEVEL + 1);
        Node* x = header;
        for (int i = level; i >= 0; i--) {
            while (x->forward[i] && x->forward[i]->key < key)
                x = x->forward[i];
            update[i] = x;
        }
        x = x->forward[0];
        if (x && x->key == key) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != x)
                    break;
                update[i]->forward[i] = x->forward[i];
            }
            delete x;
            while (level > 0 && header->forward[level] == nullptr)
                level--;
            cout << "Deleted key: " << key << endl;
        }
    }

    void display() {
        cout << "\n***** Skip List *****" << endl;
        for (int i = level; i >= 0; i--) {
            Node* x = header->forward[i];
            cout << "Level " << i << ": ";
            while (x) {
                cout << x->key << " ";
                x = x->forward[i];
            }
            cout << endl;
        }
    }

    ~SkipList() {
        Node* node = header;
        while (node) {
            Node* next = node->forward[0];
            delete node;
            node = next;
        }
    }
};

int main() {
    SkipList sl;

    sl.insert(3);
    sl.insert(6);
    sl.insert(7);
    sl.insert(9);
    sl.insert(12);
    sl.insert(19);
    sl.insert(17);
    sl.insert(26);
    sl.insert(21);
    sl.insert(25);

    sl.display();

    cout << "\nSearch for 19: " << (sl.search(19) ? "Found" : "Not Found") << endl;
    cout << "Search for 15: " << (sl.search(15) ? "Found" : "Not Found") << endl;

    sl.erase(19);
    sl.display();

    return 0;
}
