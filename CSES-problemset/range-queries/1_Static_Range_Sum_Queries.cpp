#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void solve()
{

    ll n;
    cin >> n;
    ll q;
    cin >> q;

    // ARRAY INPUT //
    vector<ll> arr(n);
    for (auto &ele : arr)
    {
        cin >> ele;
    }

    
    ll sum = 0;

    // Prefix Sum Array //
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
        arr[i] = sum;
    }

    for (int i = 0; i < q; i++)
    {
        ll l, r;
        cin >> l >> r;
        l--;
        r--;

        if (l == 0)
            cout << arr[r] << endl;
        else
            cout << arr[r] - arr[l - 1] << endl;
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