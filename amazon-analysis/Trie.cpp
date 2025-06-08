// Generic C++ implementation of Trie (Prefix Tree)
#include <unordered_map>
#include <vector>
#include <string>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    int frequency; // For ranking suggestions

    TrieNode() : isEndOfWord(false), frequency(0) {}
};

class Trie {
private:
    TrieNode* root;

    // Helper for collecting all words with a given prefix
    void collect(TrieNode* node, std::string prefix, std::vector<std::pair<std::string, int>>& results) {
        if (node->isEndOfWord) {
            results.push_back({prefix, node->frequency});
        }
        for (auto& [ch, child] : node->children) {
            collect(child, prefix + ch, results);
        }
    }

public:
    Trie() { root = new TrieNode(); }

    // Insert a word with optional frequency
    void insert(const std::string& word, int freq = 1) {
        TrieNode* node = root;
        for (char ch : word) {
            if (!node->children.count(ch))
                node->children[ch] = new TrieNode();
            node = node->children[ch];
        }
        node->isEndOfWord = true;
        node->frequency += freq;
    }

    // Find all completions for a given prefix
    std::vector<std::pair<std::string, int>> autocomplete(const std::string& prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            if (!node->children.count(ch))
                return {};
            node = node->children[ch];
        }
        std::vector<std::pair<std::string, int>> results;
        collect(node, prefix, results);
        return results;
    }
};

