// Generic C++ implementation of Segment Tree
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <functional>

template<typename T>
class SegmentTree {
private:
    std::vector<T> tree;
    std::vector<T> lazy;
    int n;
    T defaultValue;
    std::function<T(T, T)> combine;
    
    // Build the segment tree
    void build(const std::vector<T>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            build(arr, 2 * node, start, mid);
            build(arr, 2 * node + 1, mid + 1, end);
            tree[node] = combine(tree[2 * node], tree[2 * node + 1]);
        }
    }
    
    // Push lazy updates down the tree
    void push(int node, int start, int end) {
        if (lazy[node] != defaultValue) {
            tree[node] = lazy[node];
            if (start != end) {
                lazy[2 * node] = lazy[node];
                lazy[2 * node + 1] = lazy[node];
            }
            lazy[node] = defaultValue;
        }
    }
    
    // Update a range with lazy propagation
    void updateRange(int node, int start, int end, int l, int r, T value) {
        push(node, start, end);
        if (start > r || end < l) return;
        
        if (start >= l && end <= r) {
            lazy[node] = value;
            push(node, start, end);
            return;
        }
        
        int mid = (start + end) / 2;
        updateRange(2 * node, start, mid, l, r, value);
        updateRange(2 * node + 1, mid + 1, end, l, r, value);
        
        push(2 * node, start, mid);
        push(2 * node + 1, mid + 1, end);
        tree[node] = combine(tree[2 * node], tree[2 * node + 1]);
    }
    
    // Query a range
    T queryRange(int node, int start, int end, int l, int r) {
        if (start > r || end < l) return defaultValue;
        
        push(node, start, end);
        
        if (start >= l && end <= r) return tree[node];
        
        int mid = (start + end) / 2;
        T leftResult = queryRange(2 * node, start, mid, l, r);
        T rightResult = queryRange(2 * node + 1, mid + 1, end, l, r);
        
        if (leftResult == defaultValue) return rightResult;
        if (rightResult == defaultValue) return leftResult;
        return combine(leftResult, rightResult);
    }
    
    // Update a single point
    void updatePoint(int node, int start, int end, int index, T value) {
        if (start == end) {
            tree[node] = value;
        } else {
            int mid = (start + end) / 2;
            if (index <= mid) {
                updatePoint(2 * node, start, mid, index, value);
            } else {
                updatePoint(2 * node + 1, mid + 1, end, index, value);
            }
            tree[node] = combine(tree[2 * node], tree[2 * node + 1]);
        }
    }

public:
    // Constructor
    SegmentTree(const std::vector<T>& arr, std::function<T(T, T)> combineFunc, T defVal) 
        : n(arr.size()), combine(combineFunc), defaultValue(defVal) {
        tree.resize(4 * n);
        lazy.resize(4 * n, defaultValue);
        build(arr, 1, 0, n - 1);
    }
    
    // Query range [l, r]
    T query(int l, int r) {
        return queryRange(1, 0, n - 1, l, r);
    }
    
    // Update single point
    void update(int index, T value) {
        updatePoint(1, 0, n - 1, index, value);
    }
    
    // Update range [l, r] with lazy propagation
    void updateRange(int l, int r, T value) {
        updateRange(1, 0, n - 1, l, r, value);
    }
    
    // Get size
    int size() const {
        return n;
    }
};

// Specialized segment trees for different operations
class MinSegmentTree {
private:
    SegmentTree<long long> tree;
    
public:
    MinSegmentTree(const std::vector<long long>& arr) 
        : tree(arr, [](long long a, long long b) { 
            return std::min(a, b); 
        }, LLONG_MAX) {}
    
    long long rangeMin(int l, int r) {
        return tree.query(l, r);
    }
    
    void updatePrice(int index, long long price) {
        tree.update(index, price);
    }
};

class MaxSegmentTree {
private:
    SegmentTree<long long> tree;
    
public:
    MaxSegmentTree(const std::vector<long long>& arr) 
        : tree(arr, [](long long a, long long b) { 
            return std::max(a, b); 
        }, LLONG_MIN) {}
    
    long long rangeMax(int l, int r) {
        return tree.query(l, r);
    }
    
    void updatePrice(int index, long long price) {
        tree.update(index, price);
    }
};

class SumSegmentTree {
private:
    SegmentTree<long long> tree;
    
public:
    SumSegmentTree(const std::vector<long long>& arr) 
        : tree(arr, [](long long a, long long b) { 
            return a + b; 
        }, 0) {}
    
    long long rangeSum(int l, int r) {
        return tree.query(l, r);
    }
    
    void updatePrice(int index, long long price) {
        tree.update(index, price);
    }
    
    void updateRange(int l, int r, long long price) {
        tree.updateRange(l, r, price);
    }
};
