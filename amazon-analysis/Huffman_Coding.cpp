#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void buildCodes(Node* root, const string& str, unordered_map<char, string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }
    buildCodes(root->left, str + "0", huffmanCode);
    buildCodes(root->right, str + "1", huffmanCode);
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void huffmanCoding(const string& text) {
    unordered_map<char, int> freq;
    for (char ch : text) freq[ch]++;

    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& [ch, f] : freq) {
        pq.push(new Node(ch, f));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }

    Node* root = pq.top();
    unordered_map<char, string> huffmanCode;
    buildCodes(root, "", huffmanCode);

    cout << "Huffman Codes:\n";
    for (auto& [ch, code] : huffmanCode) {
        cout << ch << ": " << code << endl;
    }

    cout << "Encoded string:\n";
    string encoded = "";
    for (char ch : text) encoded += huffmanCode[ch];
    cout << encoded << endl;

    freeTree(root);
}

int main() {
    string text = "this is an example for huffman encoding";
    huffmanCoding(text);
    return 0;
}
