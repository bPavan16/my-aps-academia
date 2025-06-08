#include <iostream>
#include <string>
using namespace std;

class TernarySearchTree {
private:
    struct Node {
        char data;
        bool isEndOfWord;
        Node* left;
        Node* equal;
        Node* right;
        
        Node(char c) : data(c), isEndOfWord(false), left(nullptr), equal(nullptr), right(nullptr) {}
    };
    
    Node* root;
    
    Node* insert(Node* node, string word, int index) {
        char c = word[index];
        
        if (node == nullptr) {
            node = new Node(c);
        }
        
        if (c < node->data) {
            node->left = insert(node->left, word, index);
        }
        else if (c > node->data) {
            node->right = insert(node->right, word, index);
        }
        else {
            if (index < word.length() - 1) {
                node->equal = insert(node->equal, word, index + 1);
            }
            else {
                node->isEndOfWord = true;
            }
        }
        
        return node;
    }
    
    bool search(Node* node, string word, int index) {
        if (node == nullptr) {
            return false;
        }
        
        char c = word[index];
        
        if (c < node->data) {
            return search(node->left, word, index);
        }
        else if (c > node->data) {
            return search(node->right, word, index);
        }
        else {
            if (index == word.length() - 1) {
                return node->isEndOfWord;
            }
            return search(node->equal, word, index + 1);
        }
    }
    
    void traverse(Node* node, string prefix) {
        if (node == nullptr) {
            return;
        }
        
        traverse(node->left, prefix);
        
        if (node->isEndOfWord) {
            cout << prefix + node->data << endl;
        }
        
        traverse(node->equal, prefix + node->data);
        traverse(node->right, prefix);
    }
    
    void collectWords(Node* node, string prefix, string& result) {
        if (node == nullptr) {
            return;
        }
        
        collectWords(node->left, prefix, result);
        
        if (node->isEndOfWord) {
            result += prefix + node->data + " ";
        }
        
        collectWords(node->equal, prefix + node->data, result);
        collectWords(node->right, prefix, result);
    }
    
    Node* deleteWord(Node* node, string word, int index) {
        if (node == nullptr) {
            return nullptr;
        }
        
        char c = word[index];
        
        if (c < node->data) {
            node->left = deleteWord(node->left, word, index);
        }
        else if (c > node->data) {
            node->right = deleteWord(node->right, word, index);
        }
        else {
            if (index == word.length() - 1) {
                node->isEndOfWord = false;
            }
            else {
                node->equal = deleteWord(node->equal, word, index + 1);
            }
        }
        
        // If node has no children and is not end of word, delete it
        if (node->left == nullptr && node->right == nullptr && 
            node->equal == nullptr && !node->isEndOfWord) {
            delete node;
            return nullptr;
        }
        
        return node;
    }

public:
    TernarySearchTree() : root(nullptr) {}
    
    void insert(string word) {
        if (!word.empty()) {
            root = insert(root, word, 0);
        }
    }
    
    bool search(string word) {
        if (word.empty()) {
            return false;
        }
        return search(root, word, 0);
    }
    
    void displayAll() {
        cout << "All words in TST:" << endl;
        traverse(root, "");
    }
    
    string getAllWords() {
        string result = "";
        collectWords(root, "", result);
        return result;
    }
    
    void deleteWord(string word) {
        if (!word.empty()) {
            root = deleteWord(root, word, 0);
        }
    }
    
    bool startsWith(string prefix) {
        Node* node = root;
        int index = 0;
        
        while (node != nullptr && index < prefix.length()) {
            char c = prefix[index];
            
            if (c < node->data) {
                node = node->left;
            }
            else if (c > node->data) {
                node = node->right;
            }
            else {
                if (index == prefix.length() - 1) {
                    return true;
                }
                node = node->equal;
                index++;
            }
        }
        
        return false;
    }
    
    void wordsWithPrefix(string prefix) {
        Node* node = root;
        int index = 0;
        
        // Navigate to the end of prefix
        while (node != nullptr && index < prefix.length()) {
            char c = prefix[index];
            
            if (c < node->data) {
                node = node->left;
            }
            else if (c > node->data) {
                node = node->right;
            }
            else {
                if (index == prefix.length() - 1) {
                    break;
                }
                node = node->equal;
                index++;
            }
        }
        
        if (node != nullptr && index == prefix.length() - 1) {
            cout << "Words with prefix '" << prefix << "':" << endl;
            if (node->isEndOfWord) {
                cout << prefix << endl;
            }
            traverse(node->equal, prefix);
        }
        else {
            cout << "No words found with prefix '" << prefix << "'" << endl;
        }
    }
};

int main() {
    TernarySearchTree tst;
    
    cout << "=== Ternary Search Tree Demo ===" << endl;
    
    // Insert words
    cout << "\nInserting words..." << endl;
    tst.insert("cat");
    tst.insert("cats");
    tst.insert("up");
    tst.insert("bug");
    tst.insert("add");
    tst.insert("app");
    tst.insert("application");
    tst.insert("apply");
    tst.insert("apple");
    
    // Display all words
    cout << "\n";
    tst.displayAll();
    
    // Search operations
    cout << "\n=== Search Operations ===" << endl;
    string searchWords[] = {"cat", "car", "app", "apple", "application"};
    
    for (string word : searchWords) {
        cout << "Search '" << word << "': " << (tst.search(word) ? "Found" : "Not Found") << endl;
    }
    
    // Prefix operations
    cout << "\n=== Prefix Operations ===" << endl;
    cout << "Starts with 'app': " << (tst.startsWith("app") ? "Yes" : "No") << endl;
    cout << "Starts with 'car': " << (tst.startsWith("car") ? "Yes" : "No") << endl;
    
    cout << "\n";
    tst.wordsWithPrefix("app");
    
    cout << "\n";
    tst.wordsWithPrefix("ca");
    
    // Delete operation
    cout << "\n=== Delete Operation ===" << endl;
    cout << "Deleting 'cat'..." << endl;
    tst.deleteWord("cat");
    
    cout << "Search 'cat' after deletion: " << (tst.search("cat") ? "Found" : "Not Found") << endl;
    cout << "Search 'cats' after deletion: " << (tst.search("cats") ? "Found" : "Not Found") << endl;
    
    cout << "\nFinal word list:" << endl;
    tst.displayAll();
    
    return 0;
}
