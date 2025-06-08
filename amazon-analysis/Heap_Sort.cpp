// Generic C++ implementation of Heap Sort algorithm
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

template<typename T, typename Compare = std::less<T>>
class HeapSort {
private:
    Compare comp;
    
    // Get parent index
    int parent(int index) const {
        return (index - 1) / 2;
    }
    
    // Get left child index
    int leftChild(int index) const {
        return 2 * index + 1;
    }
    
    // Get right child index
    int rightChild(int index) const {
        return 2 * index + 2;
    }
    
    // Heapify down for max heap (used in heap sort)
    void maxHeapify(std::vector<T>& arr, int index, int heapSize) {
        int leftIdx = leftChild(index);
        int rightIdx = rightChild(index);
        int largest = index;
        
        // Find largest among parent and children
        if (leftIdx < heapSize && !comp(arr[leftIdx], arr[largest])) {
            largest = leftIdx;
        }
        
        if (rightIdx < heapSize && !comp(arr[rightIdx], arr[largest])) {
            largest = rightIdx;
        }
        
        if (largest != index) {
            std::swap(arr[index], arr[largest]);
            maxHeapify(arr, largest, heapSize);
        }
    }
    
    // Build max heap from array
    void buildMaxHeap(std::vector<T>& arr) {
        int n = arr.size();
        // Start from last non-leaf node
        for (int i = (n / 2) - 1; i >= 0; i--) {
            maxHeapify(arr, i, n);
        }
    }

public:
    // Constructor
    HeapSort(Compare c = Compare()) : comp(c) {}
    
    // Sort array using heap sort algorithm
    void sort(std::vector<T>& arr) {
        if (arr.size() <= 1) return;
        
        // Step 1: Build max heap
        buildMaxHeap(arr);
        
        // Step 2: Extract elements one by one
        for (int i = arr.size() - 1; i > 0; i--) {
            // Move current root to end
            std::swap(arr[0], arr[i]);
            
            // Reduce heap size and heapify root
            maxHeapify(arr, 0, i);
        }
        
        // If using std::greater, reverse for ascending order
        if (std::is_same_v<Compare, std::greater<T>>) {
            std::reverse(arr.begin(), arr.end());
        }
    }
    
    // Sort and return new vector
    std::vector<T> sortCopy(const std::vector<T>& arr) {
        std::vector<T> result = arr;
        sort(result);
        return result;
    }
    
    // Partial sort: find top k elements
    std::vector<T> partialSort(const std::vector<T>& arr, int k) {
        if (k <= 0) return {};
        if (k >= arr.size()) return sortCopy(arr);
        
        std::vector<T> result = arr;
        buildMaxHeap(result);
        
        std::vector<T> topK;
        int heapSize = result.size();
        
        for (int i = 0; i < k && heapSize > 0; i++) {
            topK.push_back(result[0]);
            std::swap(result[0], result[heapSize - 1]);
            heapSize--;
            maxHeapify(result, 0, heapSize);
        }
        
        // Reverse if needed for correct order
        if (std::is_same_v<Compare, std::less<T>>) {
            std::reverse(topK.begin(), topK.end());
        }
        
        return topK;
    }
    
    // Check if array is heap
    bool isMaxHeap(const std::vector<T>& arr) const {
        int n = arr.size();
        
        for (int i = 0; i <= (n - 2) / 2; i++) {
            int leftIdx = leftChild(i);
            int rightIdx = rightChild(i);
            
            if (leftIdx < n && comp(arr[i], arr[leftIdx])) {
                return false;
            }
            
            if (rightIdx < n && comp(arr[i], arr[rightIdx])) {
                return false;
            }
        }
        
        return true;
    }
    
    // Get heap height
    int getHeapHeight(int n) const {
        if (n == 0) return 0;
        return static_cast<int>(std::floor(std::log2(n))) + 1;
    }
    
    // Print heap structure
    void printHeap(const std::vector<T>& arr) const {
        if (arr.empty()) {
            std::cout << "Empty heap" << std::endl;
            return;
        }
        
        int n = arr.size();
        int height = getHeapHeight(n);
        
        std::cout << "Heap structure:" << std::endl;
        int levelStart = 0;
        
        for (int level = 0; level < height; level++) {
            int levelSize = std::min(1 << level, n - levelStart);
            
            std::cout << "Level " << level << ": ";
            for (int i = 0; i < levelSize; i++) {
                std::cout << arr[levelStart + i] << " ";
            }
            std::cout << std::endl;
            
            levelStart += levelSize;
            if (levelStart >= n) break;
        }
    }
};

// Utility functions
template<typename T>
void heapSortAscending(std::vector<T>& arr) {
    HeapSort<T, std::less<T>> sorter;
    sorter.sort(arr);
}

template<typename T>
void heapSortDescending(std::vector<T>& arr) {
    HeapSort<T, std::greater<T>> sorter;
    sorter.sort(arr);
}

template<typename T>
std::vector<T> getTopKElements(const std::vector<T>& arr, int k, bool ascending = true) {
    if (ascending) {
        HeapSort<T, std::greater<T>> sorter;
        return sorter.partialSort(arr, k);
    } else {
        HeapSort<T, std::less<T>> sorter;
        return sorter.partialSort(arr, k);
    }
}
