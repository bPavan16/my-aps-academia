#include <iostream>
#include <vector>
using namespace std;

class FenwickTree {
private:
    vector<int> tree;
    int n;

public:
    // Constructor
    FenwickTree(int size) {
        n = size;
        tree.assign(n + 1, 0);
    }

    // Constructor with initial array
    FenwickTree(vector<int>& arr) {
        n = arr.size();
        tree.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            update(i, arr[i]);
        }
    }

    // Update value at index i by adding delta
    void update(int i, int delta) {
        for (++i; i <= n; i += i & -i) {
            tree[i] += delta;
        }
    }

    // Get prefix sum from index 0 to i
    int query(int i) {
        int sum = 0;
        for (++i; i > 0; i -= i & -i) {
            sum += tree[i];
        }
        return sum;
    }

    // Get range sum from index l to r (inclusive)
    int rangeQuery(int l, int r) {
        return query(r) - query(l - 1);
    }

    // Set value at index i to val
    void set(int i, int val) {
        int currentVal = rangeQuery(i, i);
        update(i, val - currentVal);
    }

    // Print the tree (for debugging)
    void printTree() {
        cout << "Fenwick Tree: ";
        for (int i = 1; i <= n; i++) {
            cout << tree[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    // Example 1: Create empty Fenwick Tree
    cout << "=== Fenwick Tree Example ===" << endl;
    FenwickTree ft(10);

    // Update some values
    ft.update(0, 5);
    ft.update(1, 3);
    ft.update(2, 7);
    ft.update(3, 2);
    ft.update(4, 1);

    cout << "After updates:" << endl;
    ft.printTree();

    // Query operations
    cout << "Prefix sum up to index 2: " << ft.query(2) << endl;
    cout << "Prefix sum up to index 4: " << ft.query(4) << endl;
    cout << "Range sum from index 1 to 3: " << ft.rangeQuery(1, 3) << endl;

    // Example 2: Create Fenwick Tree from array
    cout << "\n=== Fenwick Tree from Array ===" << endl;
    vector<int> arr = {1, 3, 5, 7, 9, 11};
    FenwickTree ft2(arr);

    cout << "Original array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    ft2.printTree();

    cout << "Prefix sum up to index 3: " << ft2.query(3) << endl;
    cout << "Range sum from index 2 to 4: " << ft2.rangeQuery(2, 4) << endl;

    // Update and query again
    ft2.update(2, 5); // Add 5 to index 2
    cout << "After adding 5 to index 2:" << endl;
    cout << "New range sum from index 2 to 4: " << ft2.rangeQuery(2, 4) << endl;

    // Set operation
    ft2.set(1, 10); // Set index 1 to value 10
    cout << "After setting index 1 to 10:" << endl;
    cout << "New prefix sum up to index 3: " << ft2.query(3) << endl;

    return 0;
}
