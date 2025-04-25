#include <iostream>
#include <limits.h>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
    // Variables to store the input strings
    string str1, str2;

    // Input the two strings
    cout << "Enter the two strings:" << endl;
    cin >> str1 >> str2;

    // Get the lengths of the two strings
    int len1 = str1.size();
    int len2 = str2.size();

    // Create a DP table to store the lengths of the longest common substring
    int dp[len1 + 1][len2 + 1];
    int max_length = 0; // Variable to store the maximum length of the common substring

    // Initialize the DP table
    for (int i = 0; i <= len1; i++)
    {
        dp[i][0] = 0; // If str2 is empty, the length of the common substring is 0
    }
    for (int j = 0; j <= len2; j++)
    {
        dp[0][j] = 0; // If str1 is empty, the length of the common substring is 0
    }

    // Fill the DP table using bottom-up approach
    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                // Characters match, extend the common substring
                dp[i][j] = dp[i - 1][j - 1] + 1;
                max_length = max(max_length, dp[i][j]);
            }
            else
            {
                // Characters don't match, reset the common substring length
                dp[i][j] = 0;
            }
        }
    }

    // Output the length of the longest common substring
    cout << "Length of the longest common substring is: " << max_length << endl;

    return 0;
}