#include <bits/stdc++.h>
// #include<conio.h>
using namespace std;

typedef long long int ll;

void buildSegementTree(vector<ll> &arr, vector<ll> &segmentTree, ll idx, ll low, ll high)
{
    // base case : leaf node

    if (low == high)
    {
        segmentTree[idx] = arr[low];
        return;
    }

    // for non-leaf node
    ll mid = (low + high) / 2;

    // left child
    buildSegementTree(arr, segmentTree, 2 * idx + 1, low, mid);

    // right child
    buildSegementTree(arr, segmentTree, 2 * idx + 2, mid + 1, high);

    // min of left and right child gives the value of the parent node
    segmentTree[idx] = (segmentTree[2 * idx + 1] + segmentTree[2 * idx + 2]);
}

ll query(ll idx, ll low, ll high, ll l, ll r, vector<ll> &segmentTree)

{
    // if there is complete overlap
    if (low >= l && high <= r)
        return segmentTree[idx];

    if (high < l || low > r)
        return 0;

    ll mid = (low + high) / 2;

    ll leftVal = query(2 * idx + 1, low, mid, l, r, segmentTree);
    ll rightVal = query(2 * idx + 2, mid + 1, high, l, r, segmentTree);

    return (leftVal + rightVal);
}

void update(ll idx, ll low, ll high, ll pos, ll value, vector<ll> &segmentTree)
{
    // base case : leaf node
    if (low == high)
    {
        segmentTree[idx] = value;
        return;
    }

    // for non-leaf node

    ll mid = (low + high) / 2;

    if (pos <= mid)
        update(2 * idx + 1, low, mid, pos, value, segmentTree);
    else
        update(2 * idx + 2, mid + 1, high, pos, value, segmentTree);

    // min of left and right child gives the value of the parent node
    segmentTree[idx] = (segmentTree[2 * idx + 1] + segmentTree[2 * idx + 2]);
}

int main()
{

    ll n, q;
    cin >> n >> q;

    vector<ll> arr(n);
    for (ll i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    vector<ll> segmentTree(4 * n, 0);

    buildSegementTree(arr, segmentTree, 0, 0, n - 1);

    for (ll i = 0; i < q; i++)
    {
        ll type;
        cin >> type;

        if (type == 1)
        {
            ll pos, value;
            cin >> pos >> value;
            pos--;
            arr[pos] = value;
            update(0, 0, n - 1, pos, value, segmentTree);
        }
        if (type == 2)
        {
            ll l, r;
            cin >> l >> r;
            l--;
            r--;
            cout << query(0, 0, n - 1, l, r, segmentTree) << endl;
        }
    }

    // getch();
    return 0;
}

# Dynamic Range Sum Queries Analysis

**Main Task**: The code implements a dynamic range sum query system using a segment tree data structure, capable of handling both value updates and range sum calculations efficiently.

**Do we Update?**: Yes, the code includes an update function that modifies both the original array and the segment tree when element values change (type 1 queries). The update operation travels down the tree in O(log n) time, updating node values along the path.

**Do we Search?**: Yes, the query function searches the segment tree to find the sum of elements within a specified range [l,r] for type 2 queries. This search efficiently handles three cases: complete overlap, no overlap, and partial overlap.

**Do we Delete?**: No, the code doesn't remove any elements from either the array or segment tree. It only updates values at existing positions.

**Comments**: This solution utilizes a segment tree to efficiently handle dynamic array operations. After O(n) time to build the initial tree, both updates and queries run in O(log n) time, making it suitable for problems with frequent modifications. Unlike the static version, this implementation allows for value changes while maintaining efficient range sum capability. The code properly handles 1-indexed input by decrementing indices. Overall time complexity is O(n + q log n) where q is the number of queries.