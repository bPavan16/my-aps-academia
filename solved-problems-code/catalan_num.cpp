#include <iostream>
#include <cmath>
using namespace std;

// Function to calculate the nth Catalan number using recursion

// Catalan numbers are defined as C(n) = (2n)! / ((n + 1)! * n!)

unsigned long int calculate_catalan(int n) {
    // Base case: The 0th and 1st Catalan numbers are 1
    if (n <= 1) {
        return 1;
    }

    // Recursive case: Calculate the nth Catalan number
    unsigned long int result = 0;
    for (int i = 0; i < n; i++) {
        result += calculate_catalan(i) * calculate_catalan(n - i - 1);
    }
    return result;
}

int main() {
    int n;

    // Input the number of Catalan numbers to generate
    cout << "Enter the number of Catalan numbers to generate (n):" << endl;
    cin >> n;

    // Print the first n Catalan numbers
    cout << "The first " << n << " Catalan numbers are:" << endl;
    for (int i = 0; i < n; i++) {
        cout << calculate_catalan(i) << " ";
    }
    cout << endl;

    return 0;
}