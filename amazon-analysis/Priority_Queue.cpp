// Generic C++ implementation of Priority Queue using Heap
#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>

template<typename T, typename Priority, typename Compare = std::less<Priority>>
class PriorityQueue {
private:
    struct Element {
        T data;
        Priority priority;
        size_t insertionOrder;
        
        Element(const T& d, const Priority& p, size_t order)
            : data(d), priority(p), insertionOrder(order) {}
    };
    
    std::vector<Element> heap;
    Compare comp;
    size_t nextOrder;
    
    // Compare elements by priority (and insertion order for stability)
    bool isHigherPriority(const Element& a, const Element& b) const {
        if (comp(a.priority, b.priority)) return true;
        if (comp(b.priority, a.priority)) return false;
        // If priorities are equal, use insertion order for stability
        return a.insertionOrder < b.insertionOrder;
    }
    
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
    
    // Heapify up
    void heapifyUp(int index) {
        while (index > 0) {
            int parentIndex = parent(index);
            if (!isHigherPriority(heap[index], heap[parentIndex])) {
                break;
            }
            std::swap(heap[index], heap[parentIndex]);
            index = parentIndex;
        }
    }
    
    // Heapify down
    void heapifyDown(int index) {
        int size = heap.size();
        
        while (leftChild(index) < size) {
            int leftIdx = leftChild(index);
            int rightIdx = rightChild(index);
            int highestPriorityIdx = index;
            
            if (leftIdx < size && isHigherPriority(heap[leftIdx], heap[highestPriorityIdx])) {
                highestPriorityIdx = leftIdx;
            }
            
            if (rightIdx < size && isHigherPriority(heap[rightIdx], heap[highestPriorityIdx])) {
                highestPriorityIdx = rightIdx;
            }
            
            if (highestPriorityIdx == index) {
                break;
            }
            
            std::swap(heap[index], heap[highestPriorityIdx]);
            index = highestPriorityIdx;
        }
    }

public:
    // Constructor
    PriorityQueue(Compare c = Compare()) : comp(c), nextOrder(0) {}
    
    // Insert element with priority
    void push(const T& data, const Priority& priority) {
        heap.emplace_back(data, priority, nextOrder++);
        heapifyUp(heap.size() - 1);
    }
    
    // Remove and return highest priority element
    T pop() {
        if (empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        
        T result = heap[0].data;
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!empty()) {
            heapifyDown(0);
        }
        
        return result;
    }
    
    // Get highest priority element without removing
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        return heap[0].data;
    }
    
    // Get priority of top element
    const Priority& topPriority() const {
        if (empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        return heap[0].priority;
    }
    
    // Check if queue is empty
    bool empty() const {
        return heap.empty();
    }
    
    // Get queue size
    size_t size() const {
        return heap.size();
    }
    
    // Clear the queue
    void clear() {
        heap.clear();
        nextOrder = 0;
    }
    
    // Update priority of an element (expensive operation)
    bool updatePriority(const T& data, const Priority& newPriority) {
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i].data == data) {
                Priority oldPriority = heap[i].priority;
                heap[i].priority = newPriority;
                
                // Determine direction of heapify
                if (comp(newPriority, oldPriority)) {
                    heapifyUp(i);
                } else {
                    heapifyDown(i);
                }
                return true;
            }
        }
        return false;
    }
    
    // Check if element exists in queue
    bool contains(const T& data) const {
        for (const auto& element : heap) {
            if (element.data == data) {
                return true;
            }
        }
        return false;
    }
    
    // Get all elements sorted by priority
    std::vector<std::pair<T, Priority>> getAllSorted() const {
        std::vector<std::pair<T, Priority>> result;
        PriorityQueue<T, Priority, Compare> temp = *this;
        
        while (!temp.empty()) {
            Priority priority = temp.topPriority();
            T data = temp.pop();
            result.emplace_back(data, priority);
        }
        
        return result;
    }
    
    // Print queue contents
    void printQueue() const {
        std::cout << "Priority Queue contents:" << std::endl;
        for (const auto& element : heap) {
            std::cout << "Data: " << element.data 
                      << ", Priority: " << element.priority 
                      << ", Order: " << element.insertionOrder << std::endl;
        }
    }
};

// Type aliases for common priority queue types
template<typename T, typename Priority>
using MinPriorityQueue = PriorityQueue<T, Priority, std::less<Priority>>;

template<typename T, typename Priority>
using MaxPriorityQueue = PriorityQueue<T, Priority, std::greater<Priority>>;
