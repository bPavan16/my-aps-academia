// Generic C++ implementation of Binary Heap
#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>
#include <algorithm>

template<typename T, typename Compare = std::less<T>>
class BinaryHeap {
private:
    std::vector<T> heap;
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
    
    // Heapify up (restore heap property upward)
    void heapifyUp(int index) {
        while (index > 0) {
            int parentIndex = parent(index);
            if (!comp(heap[index], heap[parentIndex])) {
                break;
            }
            std::swap(heap[index], heap[parentIndex]);
            index = parentIndex;
        }
    }
    
    // Heapify down (restore heap property downward)
    void heapifyDown(int index) {
        int size = heap.size();
        
        while (leftChild(index) < size) {
            int leftIdx = leftChild(index);
            int rightIdx = rightChild(index);
            int targetIdx = index;
            
            // Find the element that should be at the top
            if (leftIdx < size && comp(heap[leftIdx], heap[targetIdx])) {
                targetIdx = leftIdx;
            }
            
            if (rightIdx < size && comp(heap[rightIdx], heap[targetIdx])) {
                targetIdx = rightIdx;
            }
            
            if (targetIdx == index) {
                break;  // Heap property satisfied
            }
            
            std::swap(heap[index], heap[targetIdx]);
            index = targetIdx;
        }
    }

public:
    // Constructor
    BinaryHeap(Compare c = Compare()) : comp(c) {}
    
    // Constructor with initial data
    BinaryHeap(const std::vector<T>& data, Compare c = Compare()) : heap(data), comp(c) {
        buildHeap();
    }
    
    // Build heap from existing data
    void buildHeap() {
        int size = heap.size();
        // Start from last non-leaf node and heapify down
        for (int i = (size / 2) - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }
    
    // Insert element into heap
    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }
    
    // Remove and return top element
    T pop() {
        if (empty()) {
            throw std::runtime_error("Heap is empty");
        }
        
        T result = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!empty()) {
            heapifyDown(0);
        }
        
        return result;
    }
    
    // Get top element without removing
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }
    
    // Check if heap is empty
    bool empty() const {
        return heap.empty();
    }
    
    // Get heap size
    size_t size() const {
        return heap.size();
    }
    
    // Clear the heap
    void clear() {
        heap.clear();
    }
    
    // Get all elements as sorted vector
    std::vector<T> extractSorted() {
        std::vector<T> result;
        BinaryHeap<T, Compare> tempHeap = *this;  // Copy heap
        
        while (!tempHeap.empty()) {
            result.push_back(tempHeap.pop());
        }
        
        return result;
    }
    
    // Update element at specific index
    void updateElement(int index, const T& newValue) {
        if (index < 0 || index >= heap.size()) {
            throw std::out_of_range("Index out of range");
        }
        
        T oldValue = heap[index];
        heap[index] = newValue;
        
        // Determine direction of heapify
        if (comp(newValue, oldValue)) {
            heapifyUp(index);
        } else {
            heapifyDown(index);
        }
    }
    
    // Find element in heap
    int find(const T& value) const {
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i] == value) {
                return i;
            }
        }
        return -1;
    }
    
    // Print heap structure
    void printHeap() const {
        std::cout << "Heap contents: ";
        for (const auto& element : heap) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
    
    // Validate heap property
    bool isValidHeap() const {
        for (int i = 0; i < heap.size(); i++) {
            int leftIdx = leftChild(i);
            int rightIdx = rightChild(i);
            
            if (leftIdx < heap.size() && comp(heap[leftIdx], heap[i])) {
                return false;
            }
            
            if (rightIdx < heap.size() && comp(heap[rightIdx], heap[i])) {
                return false;
            }
        }
        return true;
    }
};

// Type aliases for common heap types
template<typename T>
using MinHeap = BinaryHeap<T, std::less<T>>;

template<typename T>
using MaxHeap = BinaryHeap<T, std::greater<T>>;

