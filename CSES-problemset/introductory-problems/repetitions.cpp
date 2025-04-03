/*
 *Written By Pavan H. B.
 *TIME :20:19:20
 */

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define fori(i, a, n) for (ll i = a; i < n; i++)
#define forr(i, a, n) for (ll i = a; i >= 0; i--)
#define all(x) x.begin(), x.end()

#define all(v) v.begin(), v.end()
#define min3(a, b, c) min(a, min(b, c))
#define max3(a, b, c) max(a, max(b, c))
#define HIGHEST 1e12
int delRow[] = {1, -1, 0, 0}, delCol[] = {0, 0, 1, -1};

void solve()
{

    string s;
    cin >> s;

    // unordered_set<char> st;
    char prev = s[0];
    ll l = 1;
    ll maxi = 1;
    ll n = s.size();

    for (ll i = 1; i < n; i++)
    {
        if (s[i] == prev)
            l++;
        else
            l = 1;
        prev = s[i];

        maxi = max(maxi, l);
    }

    maxi = max(maxi, l);

    cout << maxi << endl;
}

int main()
{

    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    ll q = 1;
    // cin >> q;

    while (q--)
    {
        solve();
    }

    return 0;
}