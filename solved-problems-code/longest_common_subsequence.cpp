#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
    // Variables to store the input strings
    string str1, str2;

    // Input the two strings
    cout << "Enter the first string:" << endl;
    cin >> str1;
    cout << "Enter the second string:" << endl;
    cin >> str2;

    // Get the lengths of the two strings
    int len1 = str1.size();
    int len2 = str2.size();

    // Create a DP table to store the lengths of LCS
    int dp[len1 + 1][len2 + 1];

    // Initialize the DP table
    for (int i = 0; i <= len1; i++)
    {
        dp[i][0] = 0; // LCS of any string with an empty string is 0
    }
    for (int j = 0; j <= len2; j++)
    {
        dp[0][j] = 0; // LCS of any string with an empty string is 0
    }

    // Fill the DP table using bottom-up approach
    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                // Characters match, include them in LCS
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else
            {
                // Characters don't match, take the maximum of excluding one character
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Output the length of the longest common subsequence
    cout << "Length of the longest common subsequence is: " << dp[len1][len2] << endl;

    return 0;
}