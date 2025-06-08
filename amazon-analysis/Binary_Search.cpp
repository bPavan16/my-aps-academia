// Generic C++ implementation of Binary Search for prefix matching
#include <vector>
#include <string>
#include <algorithm>

// Returns the range [start, end) of words starting with the prefix
std::pair<int, int> prefixRange(const std::vector<std::string>& sortedWords, const std::string& prefix) {
    auto comp = [&prefix](const std::string& word) {
        return word.compare(0, prefix.size(), prefix) < 0;
    };
    auto lower = std::lower_bound(sortedWords.begin(), sortedWords.end(), prefix);
    std::string nextPrefix = prefix;
    if (!nextPrefix.empty()) nextPrefix.back()++;
    auto upper = std::lower_bound(sortedWords.begin(), sortedWords.end(), nextPrefix);
    return {static_cast<int>(lower - sortedWords.begin()), static_cast<int>(upper - sortedWords.begin())};
}
