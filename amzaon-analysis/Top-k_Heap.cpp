// Generic C++ implementation of Top-k Heap
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

template<typename T, typename Compare = std::less<T>>
class TopKHeap {
private:
    int k;
    Compare comp;
    // Use opposite comparison for internal heap
    std::priority_queue<T, std::vector<T>, std::conditional_t<
        std::is_same_v<Compare, std::less<T>>, 
        std::greater<T>, 
        std::less<T>>> heap;

public:
    // Constructor
    TopKHeap(int maxSize, Compare c = Compare()) : k(maxSize), comp(c) {
        if (k <= 0) {
            throw std::invalid_argument("k must be positive");
        }
    }
    
    // Add element to top-k collection
    void add(const T& element) {
        if (heap.size() < k) {
            // Heap not full, add element
            heap.push(element);
        } else if (comp(element, heap.top())) {
            // Element is better than worst in top-k
            heap.pop();
            heap.push(element);
        }
    }
    
    // Add multiple elements
    void addBatch(const std::vector<T>& elements) {
        for (const auto& element : elements) {
            add(element);
        }
    }
    
    // Get top-k elements in correct order
    std::vector<T> getTopK() const {
        std::vector<T> result;
        std::priority_queue<T, std::vector<T>, std::conditional_t<
            std::is_same_v<Compare, std::less<T>>, 
            std::greater<T>, 
            std::less<T>>> tempHeap = heap;
        
        while (!tempHeap.empty()) {
            result.push_back(tempHeap.top());
            tempHeap.pop();
        }
        
        // Reverse to get correct order
        std::reverse(result.begin(), result.end());
        return result;
    }
    
    // Get current size
    size_t size() const {
        return heap.size();
    }
    
    // Check if collection is full
    bool isFull() const {
        return heap.size() == k;
    }
    
    // Check if collection is empty
    bool empty() const {
        return heap.empty();
    }
    
    // Get the worst element in top-k (the boundary element)
    T getWorst() const {
        if (empty()) {
            throw std::runtime_error("TopKHeap is empty");
        }
        return heap.top();
    }
    
    // Get the best element without removing it
    T getBest() const {
        if (empty()) {
            throw std::runtime_error("TopKHeap is empty");
        }
        
        std::vector<T> sorted = getTopK();
        return sorted[0];
    }
    
    // Clear the collection
    void clear() {
        while (!heap.empty()) {
            heap.pop();
        }
    }
    
    // Update k value (resize collection)
    void resize(int newK) {
        if (newK <= 0) {
            throw std::invalid_argument("k must be positive");
        }
        
        if (newK < k) {
            // Reduce size: remove worst elements
            while (heap.size() > newK) {
                heap.pop();
            }
        }
        k = newK;
    }
    
    // Check if element would be in top-k
    bool wouldBeInTopK(const T& element) const {
        if (heap.size() < k) {
            return true;
        }
        return comp(element, heap.top());
    }
    
    // Get statistics about the collection
    struct Stats {
        size_t count;
        T best;
        T worst;
        bool isFull;
    };
    
    Stats getStats() const {
        if (empty()) {
            throw std::runtime_error("Cannot get stats from empty collection");
        }
        
        auto topK = getTopK();
        return Stats{
            heap.size(),
            topK[0],
            topK.back(),
            isFull()
        };
    }
};

// Specialized Top-k classes
template<typename T>
class TopKMin : public TopKHeap<T, std::less<T>> {
public:
    TopKMin(int k) : TopKHeap<T, std::less<T>>(k) {}
    
    // Get k smallest elements
    std::vector<T> getSmallest() const {
        return this->getTopK();
    }
};

template<typename T>
class TopKMax : public TopKHeap<T, std::greater<T>> {
public:
    TopKMax(int k) : TopKHeap<T, std::greater<T>>(k) {}
    
    // Get k largest elements
    std::vector<T> getLargest() const {
        return this->getTopK();
    }
};

// Utility functions for top-k operations
template<typename T>
std::vector<T> findTopKSmallest(const std::vector<T>& data, int k) {
    TopKMin<T> topK(k);
    topK.addBatch(data);
    return topK.getSmallest();
}

template<typename T>
std::vector<T> findTopKLargest(const std::vector<T>& data, int k) {
    TopKMax<T> topK(k);
    topK.addBatch(data);
    return topK.getLargest();
}
