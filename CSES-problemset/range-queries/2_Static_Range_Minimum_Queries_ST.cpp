#include <bits/stdc++.h>
// #include<conio.h>
using namespace std;

void buildSegementTree(vector<int> &arr, vector<int> &segmentTree, int idx, int low, int high)
{
    // base case : leaf node

    if (low == high)
    {
        segmentTree[idx] = arr[low];
        return;
    }

    // for non-leaf node

    int mid = (low + high) / 2;

    // left child
    buildSegementTree(arr, segmentTree, 2 * idx + 1, low, mid);

    // right child
    buildSegementTree(arr, segmentTree, 2 * idx + 2, mid + 1, high);

    // min of left and right child gives the value of the parent node
    segmentTree[idx] = min(segmentTree[2 * idx + 1], segmentTree[2 * idx + 2]);
}

int query(int idx, int low, int high, int l, int r, vector<int> &segmentTree)

{
    // if there is complete overlap
    if (low >= l && high <= r)
        return segmentTree[idx];

    if (high < l || low > r)
        return INT_MAX;

    int mid = (low + high) / 2;

    int leftVal = query(2 * idx + 1, low, mid, l, r, segmentTree);
    int rightVal = query(2 * idx + 2, mid + 1, high, l, r, segmentTree);

    return min(leftVal, rightVal);
}

int main()
{

    int n, q;
    cin >> n >> q;

    vector<int> arr(n);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    vector<int> segmentTree(4 * n, 0);

    // Build the segment tree
    buildSegementTree(arr, segmentTree, 0, 0, n - 1);

    for (int i = 0; i < q; i++)
    {
        int l, r;
        cin >> l >> r;
        l--;
        r--;
        cout << query(0, 0, n - 1, l, r, segmentTree) << endl;
    }

    // getch();
    return 0;
}

