#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

// Macro to find the minimum of two values
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Function to count the number of set bits (1s) in the binary representation of a number
int count_set_bits(int num)
{
    int count = 0;
    while (num)
    {
        if (num % 2 == 1)
        { // Check if the least significant bit is 1
            count++;
        }
        num /= 2; // Right shift the number
    }
    return count;
}

int main()
{
    int n, i, j, mask, current_task;
    int cost_matrix[1000][1000]; // Cost matrix to store task assignment costs
    int dp[1000];                // DP array to store minimum costs for each mask

    // Input the number of tasks/workers
    printf("Enter the number of tasks/workers (n):\n");
    scanf("%d", &n);

    // Input the cost matrix
    printf("Enter the cost matrix (n x n):\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            scanf("%d", &cost_matrix[i][j]);
        }
    }

    // Initialize the DP array with maximum possible values
    for (i = 0; i < (1 << n); i++)
    {
        dp[i] = INT_MAX;
    }
    dp[0] = 0; // Base case: no tasks assigned, cost is 0

    // Iterate over all possible task assignment masks
    for (mask = 0; mask < (1 << n); mask++)
    {
        current_task = count_set_bits(mask); // Determine the current task index

        for (j = 0; j < n; j++)
        {
            // If worker `j` is not assigned in the current mask
            if (!(mask & (1 << j)))
            {
                // Update the DP value for the new mask
                dp[mask | (1 << j)] = MIN(dp[mask | (1 << j)], dp[mask] + cost_matrix[current_task][j]);
            }
        }
    }

    // The answer is the minimum cost to assign all tasks
    int min_cost = dp[(1 << n) - 1];
    printf("Minimum cost to assign tasks: %d\n", min_cost);

    return 0;
}