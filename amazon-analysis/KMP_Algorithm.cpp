// C++ implementation of KMP Algorithm for Kindle real-time search
#include <iostream>
#include <vector>
#include <string>

class KindleKMPSearch {
private:
    // Compute failure function (prefix function) for KMP
    std::vector<int> computeFailureFunction(const std::string& pattern) {
        int m = pattern.length();
        std::vector<int> failure(m, 0);
        
        int j = 0;  // Length of previous longest prefix suffix
        
        for (int i = 1; i < m; i++) {
            while (j > 0 && pattern[i] != pattern[j]) {
                j = failure[j - 1];
            }
            
            if (pattern[i] == pattern[j]) {
                j++;
            }
            
            failure[i] = j;
        }
        
        return failure;
    }
    
    // Enhanced failure function with case-insensitive matching
    std::vector<int> computeFailureFunctionCaseInsensitive(const std::string& pattern) {
        int m = pattern.length();
        std::vector<int> failure(m, 0);
        
        auto toLower = [](char c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; };
        
        int j = 0;
        for (int i = 1; i < m; i++) {
            while (j > 0 && toLower(pattern[i]) != toLower(pattern[j])) {
                j = failure[j - 1];
            }
            
            if (toLower(pattern[i]) == toLower(pattern[j])) {
                j++;
            }
            
            failure[i] = j;
        }
        
        return failure;
    }

public:
    // Standard KMP search implementation
    std::vector<int> search(const std::string& text, const std::string& pattern) {
        std::vector<int> matches;
        
        if (pattern.empty() || text.empty()) return matches;
        
        int n = text.length();
        int m = pattern.length();
        
        // Compute failure function
        std::vector<int> failure = computeFailureFunction(pattern);
        
        int j = 0;  // Index for pattern
        
        for (int i = 0; i < n; i++) {
            // Handle mismatch: move pattern using failure function
            while (j > 0 && text[i] != pattern[j]) {
                j = failure[j - 1];
            }
            
            // Characters match
            if (text[i] == pattern[j]) {
                j++;
            }
            
            // Full pattern match found
            if (j == m) {
                matches.push_back(i - m + 1);
                j = failure[j - 1];  // Continue searching for overlapping matches
            }
        }
        
        return matches;
    }
    
    // Case-insensitive KMP search for better user experience
    std::vector<int> searchCaseInsensitive(const std::string& text, const std::string& pattern) {
        std::vector<int> matches;
        
        if (pattern.empty() || text.empty()) return matches;
        
        int n = text.length();
        int m = pattern.length();
        
        auto toLower = [](char c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; };
        
        // Compute failure function for case-insensitive search
        std::vector<int> failure = computeFailureFunctionCaseInsensitive(pattern);
        
        int j = 0;
        
        for (int i = 0; i < n; i++) {
            while (j > 0 && toLower(text[i]) != toLower(pattern[j])) {
                j = failure[j - 1];
            }
            
            if (toLower(text[i]) == toLower(pattern[j])) {
                j++;
            }
            
            if (j == m) {
                matches.push_back(i - m + 1);
                j = failure[j - 1];
            }
        }
        
        return matches;
    }
    
    // Streaming KMP search for real-time search as user types
    class StreamingKMP {
    private:
        std::string pattern;
        std::vector<int> failure;
        int patternIndex;
        int textPosition;
        
    public:
        StreamingKMP(const std::string& pat) : pattern(pat), patternIndex(0), textPosition(0) {
            KindleKMPSearch kmp;
            failure = kmp.computeFailureFunction(pattern);
        }
        
        // Process one character at a time
        bool processChar(char c) {
            while (patternIndex > 0 && c != pattern[patternIndex]) {
                patternIndex = failure[patternIndex - 1];
            }
            
            if (c == pattern[patternIndex]) {
                patternIndex++;
            }
            
            textPosition++;
            
            if (patternIndex == pattern.length()) {
                patternIndex = failure[patternIndex - 1];
                return true;  // Match found
            }
            
            return false;
        }
        
        // Get current match position if found
        int getLastMatchPosition() const {
            return textPosition - pattern.length();
        }
        
        // Reset for new search
        void reset() {
            patternIndex = 0;
            textPosition = 0;
        }
    };
    
    // Fuzzy KMP search with allowed mismatches (for typo tolerance)
    std::vector<int> searchWithMismatches(const std::string& text, const std::string& pattern, int maxMismatches) {
        std::vector<int> matches;
        
        int n = text.length();
        int m = pattern.length();
        
        if (m == 0 || n == 0) return matches;
        
        // Dynamic programming approach for fuzzy matching
        for (int i = 0; i <= n - m; i++) {
            int mismatches = 0;
            
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    mismatches++;
                    if (mismatches > maxMismatches) break;
                }
            }
            
            if (mismatches <= maxMismatches) {
                matches.push_back(i);
            }
        }
        
        return matches;
    }
    
    // Word boundary search (for whole word matching)
    std::vector<int> searchWholeWords(const std::string& text, const std::string& pattern) {
        std::vector<int> allMatches = search(text, pattern);
        std::vector<int> wordMatches;
        
        auto isWordChar = [](char c) { 
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
                   (c >= '0' && c <= '9') || c == '_'; 
        };
        
        for (int pos : allMatches) {
            bool validStart = (pos == 0) || !isWordChar(text[pos - 1]);
            bool validEnd = (pos + pattern.length() == text.length()) || 
                              !isWordChar(text[pos + pattern.length()]);
            
            if (validStart && validEnd) {
                wordMatches.push_back(pos);
            }
        }
        
        return wordMatches;
    }
};
