// Generic C++ implementation of Price Query System using Segment Trees
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>

struct PriceData {
    long long price;
    int productId;
    long long timestamp;
    
    PriceData(long long p = 0, int id = 0, long long ts = 0) 
        : price(p), productId(id), timestamp(ts) {}
};

class PriceQuerySystem {
private:
    std::vector<long long> prices;
    std::unordered_map<int, int> productToIndex;
    std::unique_ptr<MinSegmentTree> minTree;
    std::unique_ptr<MaxSegmentTree> maxTree;
    std::unique_ptr<SumSegmentTree> sumTree;
    int size;
    
    // Update all segment trees when price changes
    void updateAllTrees(int index, long long newPrice) {
        prices[index] = newPrice;
        minTree->updatePrice(index, newPrice);
        maxTree->updatePrice(index, newPrice);
        sumTree->updatePrice(index, newPrice);
    }

public:
    // Constructor
    PriceQuerySystem(const std::vector<PriceData>& initialData) {
        size = initialData.size();
        prices.resize(size);
        
        // Extract prices and build mapping
        for (int i = 0; i < size; i++) {
            prices[i] = initialData[i].price;
            productToIndex[initialData[i].productId] = i;
        }
        
        // Initialize segment trees
        minTree = std::make_unique<MinSegmentTree>(prices);
        maxTree = std::make_unique<MaxSegmentTree>(prices);
        sumTree = std::make_unique<SumSegmentTree>(prices);
    }
    
    // Query minimum price in range [left, right]
    long long getMinPrice(int left, int right) {
        if (left < 0 || right >= size || left > right) return -1;
        return minTree->rangeMin(left, right);
    }
    
    // Query maximum price in range [left, right]
    long long getMaxPrice(int left, int right) {
        if (left < 0 || right >= size || left > right) return -1;
        return maxTree->rangeMax(left, right);
    }
    
    // Query sum of prices in range [left, right]
    long long getSumPrice(int left, int right) {
        if (left < 0 || right >= size || left > right) return -1;
        return sumTree->rangeSum(left, right);
    }
    
    // Get average price in range
    double getAveragePrice(int left, int right) {
        if (left < 0 || right >= size || left > right) return -1.0;
        long long sum = getSumPrice(left, right);
        int count = right - left + 1;
        return (double)sum / count;
    }
    
    // Update price for a specific product
    bool updateProductPrice(int productId, long long newPrice) {
        auto it = productToIndex.find(productId);
        if (it == productToIndex.end()) return false;
        
        int index = it->second;
        updateAllTrees(index, newPrice);
        return true;
    }
    
    // Update price by index
    bool updatePriceByIndex(int index, long long newPrice) {
        if (index < 0 || index >= size) return false;
        updateAllTrees(index, newPrice);
        return true;
    }
    
    // Bulk update prices in range
    void bulkUpdateRange(int left, int right, long long newPrice) {
        if (left < 0 || right >= size || left > right) return;
        
        // Update prices array
        for (int i = left; i <= right; i++) {
            prices[i] = newPrice;
        }
        
        // Use lazy propagation for efficient bulk updates
        sumTree->updateRange(left, right, newPrice);
        
        // For min/max trees, we need to update individually or rebuild
        for (int i = left; i <= right; i++) {
            minTree->updatePrice(i, newPrice);
            maxTree->updatePrice(i, newPrice);
        }
    }
    
    // Find products in price range
    std::vector<int> findProductsInPriceRange(long long minPrice, long long maxPrice) {
        std::vector<int> result;
        
        for (int i = 0; i < size; i++) {
            if (prices[i] >= minPrice && prices[i] <= maxPrice) {
                result.push_back(i);
            }
        }
        
        return result;
    }
    
    // Count products in price range
    int countProductsInPriceRange(long long minPrice, long long maxPrice) {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (prices[i] >= minPrice && prices[i] <= maxPrice) {
                count++;
            }
        }
        return count;
    }
    
    // Get price statistics for a range
    struct PriceStats {
        long long minPrice;
        long long maxPrice;
        long long totalPrice;
        double averagePrice;
        int productCount;
    };
    
    PriceStats getRangeStatistics(int left, int right) {
        PriceStats stats;
        
        if (left < 0 || right >= size || left > right) {
            stats = {-1, -1, -1, -1.0, 0};
            return stats;
        }
        
        stats.minPrice = getMinPrice(left, right);
        stats.maxPrice = getMaxPrice(left, right);
        stats.totalPrice = getSumPrice(left, right);
        stats.productCount = right - left + 1;
        stats.averagePrice = (double)stats.totalPrice / stats.productCount;
        
        return stats;
    }
    
    // Get current price by index
    long long getPrice(int index) const {
        if (index < 0 || index >= size) return -1;
        return prices[index];
    }
    
    // Get system size
    int getSize() const {
        return size;
    }
    
    // Print range statistics
    void printRangeStatistics(int left, int right) const {
        if (left < 0 || right >= size || left > right) {
            std::cout << "Invalid range!" << std::endl;
            return;
        }
        
        std::cout << "Price Statistics for range [" << left << ", " << right << "]:" << std::endl;
        std::cout << "  Minimum Price: $" << minTree->rangeMin(left, right) << std::endl;
        std::cout << "  Maximum Price: $" << maxTree->rangeMax(left, right) << std::endl;
        std::cout << "  Total Value: $" << sumTree->rangeSum(left, right) << std::endl;
        std::cout << "  Average Price: $" << getAveragePrice(left, right) << std::endl;
        std::cout << "  Product Count: " << (right - left + 1) << std::endl;
    }
};
