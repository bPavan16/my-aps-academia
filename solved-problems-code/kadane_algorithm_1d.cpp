#include <iostream>
#include <climits>
using namespace std;

// Function to find the maximum subarray sum using Kadane's Algorithm
// It also tracks the start and end indices of the subarray
unsigned long int kadane_algorithm(int arr[], int size, int &start_index, int &end_index)
{
    int max_sum = INT_MIN; // Stores the maximum subarray sum
    int current_sum = 0;   // Tracks the current subarray sum
    int temp_start = 0;    // Temporary start index for the current subarray

    for (int i = 0; i < size; i++)
    {
        current_sum += arr[i];

        // If the current sum becomes negative, reset it and update the temporary start index
        if (current_sum < 0)
        {
            current_sum = 0;
            temp_start = i + 1;
        }

        // Update the maximum sum and the start/end indices if a new maximum is found
        if (max_sum < current_sum)
        {
            max_sum = current_sum;
            start_index = temp_start;
            end_index = i;
        }
    }

    return max_sum;
}

int main()
{
    int n;

    // Input the size of the array
    cout << "Enter the size of the array (n):" << endl;
    cin >> n;

    int arr[n];

    // Input the elements of the array
    cout << "Enter the elements of the array:" << endl;
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    int start_index = 0, end_index = 0;

    // Find the maximum subarray sum and its indices
    unsigned long int max_sum = kadane_algorithm(arr, n, start_index, end_index);

    // Output the results
    cout << "Largest subarray sum is: " << max_sum << endl;
    cout << "Start index: " << start_index << ", End index: " << end_index << endl;

    return 0;
}