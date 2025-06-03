// C++ implementation of Suffix Tree for Kindle text search
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

class KindleSuffixTree {
private:
    struct SuffixTreeNode {
        std::unordered_map<char, SuffixTreeNode*> children;
        int start;           // Start index of edge label
        int* end;            // End index of edge label (pointer for global end)
        SuffixTreeNode* suffixLink;  // Suffix link for Ukkonen's algorithm
        int suffixIndex;    // Suffix index for leaf nodes
        
        SuffixTreeNode(int start, int* end) 
            : start(start), end(end), suffixLink(nullptr), suffixIndex(-1) {}
    };
    
    std::string text;
    SuffixTreeNode* root;
    SuffixTreeNode* lastNewNode;
    SuffixTreeNode* activeNode;
    int activeEdge;
    int activeLength;
    int remainingSuffixCount;
    int leafEnd;
    int* rootEnd;
    int* splitEnd;
    int size;
    
    // Create a new node
    SuffixTreeNode* newNode(int start, int* end) {
        SuffixTreeNode* node = new SuffixTreeNode(start, end);
        return node;
    }
    
    // Get edge length
    int edgeLength(SuffixTreeNode* node) {
        if (node == root) return 0;
        return *(node->end) - (node->start) + 1;
    }
    
    // Walk down the tree
    int walkDown(SuffixTreeNode* currentNode) {
        if (activeLength >= edgeLength(currentNode)) {
            activeEdge += edgeLength(currentNode);
            activeLength -= edgeLength(currentNode);
            activeNode = currentNode;
            return 1;
        }
        return 0;
    }
    
    // Extend the suffix tree with new character
    void extendSuffixTree(int pos) {
        leafEnd = pos;
        remainingSuffixCount++;
        lastNewNode = nullptr;
        
        while (remainingSuffixCount > 0) {
            if (activeLength == 0) {
                activeEdge = pos;
            }
            
            if (activeNode->children.find(text[activeEdge]) == activeNode->children.end()) {
                // No outgoing edge starting with activeEdge character
                activeNode->children[text[activeEdge]] = newNode(pos, &leafEnd);
                
                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }
            } else {
                // Outgoing edge exists
                SuffixTreeNode* next = activeNode->children[text[activeEdge]];
                
                if (walkDown(next)) {
                    continue;
                }
                
                // Check if current character is already on the edge
                if (text[next->start + activeLength] == text[pos]) {
                    if (lastNewNode != nullptr && activeNode != root) {
                        lastNewNode->suffixLink = activeNode;
                        lastNewNode = nullptr;
                    }
                    activeLength++;
                    break;
                }
                
                // Split the edge
                splitEnd = new int;
                *splitEnd = next->start + activeLength - 1;
                
                SuffixTreeNode* split = newNode(next->start, splitEnd);
                activeNode->children[text[activeEdge]] = split;
                
                split->children[text[pos]] = newNode(pos, &leafEnd);
                next->start += activeLength;
                split->children[text[next->start]] = next;
                
                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = split;
                }
                lastNewNode = split;
            }
            
            remainingSuffixCount--;
            
            if (activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge = pos - remainingSuffixCount + 1;
            } else if (activeNode != root) {
                activeNode = activeNode->suffixLink;
            }
        }
    }
    
    // Set suffix indices for leaf nodes
    void setSuffixIndexByDFS(SuffixTreeNode* node, int labelHeight) {
        if (node == nullptr) return;
        
        int leaf = 1;
        for (auto& child : node->children) {
            if (child.second != nullptr) {
                leaf = 0;
                setSuffixIndexByDFS(child.second, labelHeight + edgeLength(child.second));
            }
        }
        
        if (leaf == 1) {
            node->suffixIndex = size - labelHeight;
        }
    }

public:
    KindleSuffixTree() {
        rootEnd = new int;
        *rootEnd = -1;
        root = newNode(-1, rootEnd);
        activeNode = root;
        activeEdge = -1;
        activeLength = 0;
        remainingSuffixCount = 0;
        leafEnd = -1;
        size = 0;
    }
    
    // Build suffix tree for given text
    void buildSuffixTree(const std::string& txt) {
        text = txt + "$";  // Add termination character
        size = text.length();
        
        for (int i = 0; i < size; i++) {
            extendSuffixTree(i);
        }
        
        // Set suffix indices
        setSuffixIndexByDFS(root, 0);
    }
    
    // Search for pattern in the suffix tree
    std::vector<int> searchPattern(const std::string& pattern) {
        std::vector<int> results;
        
        SuffixTreeNode* currentNode = root;
        int patternIndex = 0;
        int edgeIndex = 0;
        
        while (patternIndex < pattern.length()) {
            if (currentNode->children.find(pattern[patternIndex]) == currentNode->children.end()) {
                return results;  // Pattern not found
            }
            
            SuffixTreeNode* nextNode = currentNode->children[pattern[patternIndex]];
            int edgeLength = *(nextNode->end) - nextNode->start + 1;
            
            // Traverse the edge
            for (edgeIndex = 0; edgeIndex < edgeLength && patternIndex < pattern.length(); edgeIndex++, patternIndex++) {
                if (text[nextNode->start + edgeIndex] != pattern[patternIndex]) {
                    return results;  // Pattern not found
                }
            }
            
            currentNode = nextNode;
        }
        
        // Pattern found, collect all suffix indices
        collectSuffixIndices(currentNode, results);
        return results;
    }
    
private:
    // Collect all suffix indices under a node
    void collectSuffixIndices(SuffixTreeNode* node, std::vector<int>& results) {
        if (node == nullptr) return;
        
        if (node->suffixIndex != -1) {
            results.push_back(node->suffixIndex);
        }
        
        for (auto& child : node->children) {
            collectSuffixIndices(child.second, results);
        }
    }
};

