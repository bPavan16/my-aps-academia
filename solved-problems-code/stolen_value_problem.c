#include <stdio.h>
#include <stdlib.h>

// Function to find the maximum of two integers
int max(int a, int b)
{
    return (a > b) ? a : b;
}

int main()
{
    int n;

    // Input the number of houses
    printf("Enter the number of houses (n):\n");
    scanf("%d", &n);

    // Array to store the values of items in each house
    int values[n];

    // Input the values of items in each house
    printf("Enter the values of items in the houses:\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &values[i]);
    }

    // Edge case: If there are no houses
    if (n == 0)
    {
        printf("Maximum value is: 0\n");
        return 0;
    }

    // Edge case: If there is only one house
    if (n == 1)
    {
        printf("Maximum value is: %d\n", values[0]);
        return 0;
    }

    // Dynamic programming array to store the maximum value stolen up to each house
    int *dp = (int *)malloc(n * sizeof(int));

    // Base cases
    dp[0] = values[0];                 // Maximum value for the first house
    dp[1] = max(values[0], values[1]); // Maximum value for the first two houses

    // Fill the DP array using the recurrence relation
    for (int i = 2; i < n; i++)
    {
        dp[i] = max(dp[i - 1], dp[i - 2] + values[i]);
    }

    // Output the maximum value that can be stolen
    printf("Maximum value that can be stolen is: %d\n", dp[n - 1]);

    // Free allocated memory
    free(dp);

    return 0;
}