#include <bits/stdc++.h>
// #include<conio.h>
using namespace std;

int main()
{

    int n, q;
    cin >> n >> q;

    vector<int> arr(n);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    int xorval = 0;
    for (int i = 0; i < n; i++)
    {
        xorval ^= arr[i];
        arr[i] = xorval;
    }

    for (int i = 0; i < q; i++)
    {
        int l, r;
        cin >> l >> r;
        l--;
        r--;

        if (l == 0)
            cout << arr[r] << endl;
        else
            cout << (arr[r] ^ arr[l - 1]) << endl;
    }

    // getch();
    return 0;
}