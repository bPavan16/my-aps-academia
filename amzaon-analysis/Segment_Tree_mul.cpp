// Advanced Segment Tree with multiple operations and optimizations
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

struct NodeData {
    long long minVal;
    long long maxVal;
    long long sum;
    int count;
    
    NodeData() : minVal(LLONG_MAX), maxVal(LLONG_MIN), sum(0), count(0) {}
    
    NodeData(long long val) : minVal(val), maxVal(val), sum(val), count(1) {}
    
    // Combine two nodes
    NodeData operator+(const NodeData& other) const {
        NodeData result;
        result.minVal = std::min(minVal, other.minVal);
        result.maxVal = std::max(maxVal, other.maxVal);
        result.sum = sum + other.sum;
        result.count = count + other.count;
        return result;
    }
};

class AdvancedSegmentTree {
private:
    std::vector<NodeData> tree;
    std::vector<long long> lazy;
    std::vector<bool> hasLazy;
    int n;
    
    // Build the tree
    void build(const std::vector<long long>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = NodeData(arr[start]);
        } else {
            int mid = (start + end) / 2;
            build(arr, 2 * node, start, mid);
            build(arr, 2 * node + 1, mid + 1, end);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }
    
    // Push lazy updates
    void push(int node, int start, int end) {
        if (hasLazy[node]) {
            long long val = lazy[node];
            int length = end - start + 1;
            
            tree[node].minVal = val;
            tree[node].maxVal = val;
            tree[node].sum = val * length;
            tree[node].count = length;
            
            if (start != end) {
                lazy[2 * node] = val;
                lazy[2 * node + 1] = val;
                hasLazy[2 * node] = true;
                hasLazy[2 * node + 1] = true;
            }
            
            hasLazy[node] = false;
        }
    }
    
    // Range update with lazy propagation
    void updateRange(int node, int start, int end, int l, int r, long long val) {
        push(node, start, end);
        
        if (start > r || end < l) return;
        
        if (start >= l && end <= r) {
            lazy[node] = val;
            hasLazy[node] = true;
            push(node, start, end);
            return;
        }
        
        int mid = (start + end) / 2;
        updateRange(2 * node, start, mid, l, r, val);
        updateRange(2 * node + 1, mid + 1, end, l, r, val);
        
        push(2 * node, start, mid);
        push(2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
    
    // Range query
    NodeData queryRange(int node, int start, int end, int l, int r) {
        if (start > r || end < l) return NodeData();
        
        push(node, start, end);
        
        if (start >= l && end <= r) return tree[node];
        
        int mid = (start + end) / 2;
        NodeData leftResult = queryRange(2 * node, start, mid, l, r);
        NodeData rightResult = queryRange(2 * node + 1, mid + 1, end, l, r);
        
        if (leftResult.count == 0) return rightResult;
        if (rightResult.count == 0) return leftResult;
        return leftResult + rightResult;
    }
    
    // Point update
    void updatePoint(int node, int start, int end, int index, long long val) {
        if (start == end) {
            tree[node] = NodeData(val);
        } else {
            int mid = (start + end) / 2;
            if (index <= mid) {
                updatePoint(2 * node, start, mid, index, val);
            } else {
                updatePoint(2 * node + 1, mid + 1, end, index, val);
            }
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

public:
    // Constructor
    AdvancedSegmentTree(const std::vector<long long>& arr) : n(arr.size()) {
        tree.resize(4 * n);
        lazy.resize(4 * n);
        hasLazy.resize(4 * n, false);
        build(arr, 1, 0, n - 1);
    }
    
    // Get minimum in range [l, r]
    long long rangeMin(int l, int r) {
        NodeData result = queryRange(1, 0, n - 1, l, r);
        return result.count > 0 ? result.minVal : LLONG_MAX;
    }
    
    // Get maximum in range [l, r]
    long long rangeMax(int l, int r) {
        NodeData result = queryRange(1, 0, n - 1, l, r);
        return result.count > 0 ? result.maxVal : LLONG_MIN;
    }
    
    // Get sum in range [l, r]
    long long rangeSum(int l, int r) {
        NodeData result = queryRange(1, 0, n - 1, l, r);
        return result.sum;
    }
    
    // Get count in range [l, r]
    int rangeCount(int l, int r) {
        NodeData result = queryRange(1, 0, n - 1, l, r);
        return result.count;
    }
    
    // Get average in range [l, r]
    double rangeAverage(int l, int r) {
        NodeData result = queryRange(1, 0, n - 1, l, r);
        return result.count > 0 ? (double)result.sum / result.count : 0.0;
    }
    
    // Update single point
    void update(int index, long long val) {
        updatePoint(1, 0, n - 1, index, val);
    }
    
    // Update range [l, r] to value val
    void updateRange(int l, int r, long long val) {
        updateRange(1, 0, n - 1, l, r, val);
    }
    
    // Get all statistics for range [l, r]
    NodeData getRangeData(int l, int r) {
        return queryRange(1, 0, n - 1, l, r);
    }
    
    // Binary search for first element >= target in range [l, r]
    int lowerBound(int l, int r, long long target) {
        int left = l, right = r, result = -1;
        
        while (left <= right) {
            int mid = (left + right) / 2;
            NodeData data = queryRange(1, 0, n - 1, mid, mid);
            
            if (data.count > 0 && data.minVal >= target) {
                result = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        return result;
    }
    
    // Find k-th smallest element in range [l, r]
    long long kthSmallest(int l, int r, int k) {
        // This is a simplified version - actual implementation would need
        // a more sophisticated approach or additional data structures
        std::vector<long long> values;
        
        for (int i = l; i <= r; i++) {
            NodeData data = queryRange(1, 0, n - 1, i, i);
            if (data.count > 0) {
                values.push_back(data.minVal);
            }
        }
        
        std::sort(values.begin(), values.end());
        return (k > 0 && k <= values.size()) ? values[k - 1] : LLONG_MIN;
    }
};
