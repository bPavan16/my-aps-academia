// C++ implementation of Suffix Array for efficient Kindle search
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class KindleSuffixArray {
private:
    std::string text;
    std::vector<int> suffixArray;
    std::vector<int> lcp;  // Longest Common Prefix array
    
    // Radix sort for suffix array construction
    void radixSort(std::vector<int>& suffixes, std::vector<int>& rank, int n, int maxRank) {
        std::vector<int> count(maxRank + 1, 0);
        std::vector<int> temp(n);
        
        // Sort by second element (rank[i + gap])
        for (int i = 0; i < n; i++) {
            count[rank[suffixes[i]]]++;
        }
        
        for (int i = 1; i <= maxRank; i++) {
            count[i] += count[i - 1];
        }
        
        for (int i = n - 1; i >= 0; i--) {
            temp[--count[rank[suffixes[i]]]] = suffixes[i];
        }
        
        suffixes = temp;
    }
    
    // Build suffix array using O(n log n) algorithm
    void buildSuffixArray() {
        int n = text.length();
        suffixArray.resize(n);
        
        // Initialize suffix array with indices
        for (int i = 0; i < n; i++) {
            suffixArray[i] = i;
        }
        
        // Sort suffixes using doubling technique
        std::vector<int> rank(n), tempRank(n);
        
        // Initial ranking based on first character
        for (int i = 0; i < n; i++) {
            rank[i] = text[i];
        }
        
        for (int gap = 1; gap < n; gap *= 2) {
            // Sort based on rank of suffix starting at i+gap
            auto cmp = [&](int a, int b) {
                if (rank[a] != rank[b]) return rank[a] < rank[b];
                int ra = (a + gap < n) ? rank[a + gap] : -1;
                int rb = (b + gap < n) ? rank[b + gap] : -1;
                return ra < rb;
            };
            
            std::sort(suffixArray.begin(), suffixArray.end(), cmp);
            
            // Update ranks
            tempRank[suffixArray[0]] = 0;
            for (int i = 1; i < n; i++) {
                if (cmp(suffixArray[i-1], suffixArray[i])) {
                    tempRank[suffixArray[i]] = tempRank[suffixArray[i-1]] + 1;
                } else {
                    tempRank[suffixArray[i]] = tempRank[suffixArray[i-1]];
                }
            }
            
            rank = tempRank;
            
            if (rank[suffixArray[n-1]] == n-1) break;  // All suffixes sorted
        }
    }
    
    // Build LCP (Longest Common Prefix) array
    void buildLCPArray() {
        int n = text.length();
        lcp.resize(n);
        std::vector<int> invSuffix(n);
        
        // Create inverse suffix array
        for (int i = 0; i < n; i++) {
            invSuffix[suffixArray[i]] = i;
        }
        
        int k = 0;
        for (int i = 0; i < n; i++) {
            if (invSuffix[i] == n - 1) {
                k = 0;
                continue;
            }
            
            int j = suffixArray[invSuffix[i] + 1];
            
            while (i + k < n && j + k < n && text[i + k] == text[j + k]) {
                k++;
            }
            
            lcp[invSuffix[i]] = k;
            
            if (k > 0) k--;
        }
    }

public:
    // Constructor
    KindleSuffixArray(const std::string& txt) : text(txt + "$") {
        buildSuffixArray();
        buildLCPArray();
    }
    
    // Binary search for pattern in suffix array
    std::pair<int, int> searchRange(const std::string& pattern) {
        int n = suffixArray.size();
        int patternLen = pattern.length();
        
        // Find leftmost occurrence
        int left = 0, right = n - 1, leftBound = -1;
        while (left <= right) {
            int mid = (left + right) / 2;
            int suffixStart = suffixArray[mid];
            
            int cmp = 0;
            for (int i = 0; i < patternLen && suffixStart + i < text.length() - 1; i++) {
                if (text[suffixStart + i] < pattern[i]) {
                    cmp = -1;
                    break;
                } else if (text[suffixStart + i] > pattern[i]) {
                    cmp = 1;
                    break;
                }
            }
            
            if (cmp <= 0) {
                if (cmp == 0) leftBound = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        if (leftBound == -1) return {-1, -1};
        
        // Find rightmost occurrence
        left = 0; right = n - 1; int rightBound = -1;
        while (left <= right) {
            int mid = (left + right) / 2;
            int suffixStart = suffixArray[mid];
            
            int cmp = 0;
            for (int i = 0; i < patternLen && suffixStart + i < text.length() - 1; i++) {
                if (text[suffixStart + i] < pattern[i]) {
                    cmp = -1;
                    break;
                } else if (text[suffixStart + i] > pattern[i]) {
                    cmp = 1;
                    break;
                }
            }
            
            if (cmp >= 0) {
                if (cmp == 0) rightBound = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        return {leftBound, rightBound};
    }
    
    // Get all occurrences of pattern
    std::vector<int> findAllOccurrences(const std::string& pattern) {
        std::vector<int> occurrences;
        auto range = searchRange(pattern);
        
        if (range.first == -1) return occurrences;
        
        for (int i = range.first; i <= range.second; i++) {
            occurrences.push_back(suffixArray[i]);
        }
        
        std::sort(occurrences.begin(), occurrences.end());
        return occurrences;
    }
    
    // Display suffix array (for debugging)
    void printSuffixArray() {
        std::cout << "Suffix Array:" << std::endl;
        for (int i = 0; i < suffixArray.size(); i++) {
            std::cout << suffixArray[i] << ": " << text.substr(suffixArray[i]) << std::endl;
        }
    }
};

