/*
 *Written By Pavan H. B.
 *TIME :20:19:20
 */

#include <bits/stdc++.h>
using namespace std;

#define ll long long int
#define fori(i, a, n) for (ll i = a; i < n; i++)
#define forr(i, a, n) for (ll i = a; i >= 0; i--)

#define SORT(all, ARRAY) sort(ARRAY.begin(), ARRAY.end())
#define SORTR(all, ARRAY) sort(ARRAY.rbegin(), ARRAY.rend())
#define ADD(all, ARRAY) accumulate(ARRAY.rbegin(), ARRAY.rend(), 0)
#define all(v) v.begin(), v.end()
#define min3(a, b, c) min(a, min(b, c))
#define max3(a, b, c) max(a, max(b, c))

void solve()
{

    ll n;
    cin >> n;

    // ARRAY INPUT //

    vector<ll> arr(n + 1, 0);

    for (int i = 0; i < n; i++)
    {
        int ele;
        cin >> ele;
        arr[ele]++;
    }

    for (int i = 1; i < n + 1; i++)
    {
        if (arr[i] == 0)
            cout << i << endl;
    }
}

int main()
{

    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    ll q = 1;
    while (q--)
    {
        solve();
    }

    return 0;
}
