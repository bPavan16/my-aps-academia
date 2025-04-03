#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void update(vector<ll> &fenwick, int idx, ll val, int n)
{

    while (idx <= n)
    {
        fenwick[idx] += val;
        idx += (idx & -idx);
    }
}

ll query(vector<ll> &fenwick, int idx)
{
    ll sum = 0;
    // Traverse the Fenwick tree to get the sum up to idx
    while (idx > 0)
    {
        sum += fenwick[idx];
        idx -= (idx & -idx);
    }
    return sum;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    vector<int> arr(n + 1, 0);
    for (int i = 1; i <= n; i++)
    {
        cin >> arr[i];
    }

    // Create a Fenwick tree for difference array
    vector<ll> fenwickTree(n + 2, 0);

    for (int i = 1; i <= n; i++)
    {
        // Update Fenwick tree with the initial values
        // Add arr[i] at position i to set the initial value
        update(fenwickTree, i, arr[i], n + 1);

        // Subtract arr[i] at position i+1 to maintain the difference
        // This ensures that the range update is applied correctly
        update(fenwickTree, i + 1, -arr[i], n + 1);
    }

    while (q--)
    {
        int type;
        cin >> type;

        if (type == 1)
        {
            // type 1: increase each value in range [a,b] by u
            int a, b, u;
            cin >> a >> b >> u;

            // Add u at position a so that all elements from a to b are increased by u
            update(fenwickTree, a, u, n + 1);

            // Adjust the difference array to apply the range update [a, b] by u
            // Add u at position a to start the increment from index a
            // Subtract u at position b+1 to stop the increment after index b

            update(fenwickTree, b + 1, -u, n + 1);
        }
        else
        {
            // type 2: value at position k
            int k;
            cin >> k;
            cout << query(fenwickTree, k) << "\n";
        }
    }

    return 0;
}