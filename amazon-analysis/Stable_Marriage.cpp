#include <iostream>
#include <vector>
#include <queue>
using namespace std;

bool prefersOverCurrent(int woman, int man, int currentPartner,
                        vector<vector<int>>& preferences, int n) {
    // A smaller index in the preference list means higher preference
    for (int i = 0; i < n; i++) {
        if (preferences[woman][i] == man) return true;
        if (preferences[woman][i] == currentPartner) return false;
    }
    return false;
}

int main() {
    // Example with n men and n women
    int n = 4;
    
    // Each row in menPref is a man's preference list in descending order of preference
    // menPref[man][i] = woman
    vector<vector<int>> menPref = {
        {0, 1, 2, 3},
        {1, 0, 3, 2},
        {3, 2, 0, 1},
        {2, 3, 1, 0}
    };
    
    // Each row in womenPref is a woman's preference list in descending order of preference
    // womenPref[woman][i] = man
    vector<vector<int>> womenPref = {
        {1, 0, 3, 2},
        {3, 2, 0, 1},
        {0, 2, 1, 3},
        {2, 1, 3, 0}
    };
    
    // Next man to propose index
    vector<int> nextWomanIndex(n, 0);
    // Initially no one is engaged
    vector<int> manPartner(n, -1);
    vector<int> womanPartner(n, -1);
    
    // Queue of free men
    queue<int> freeMen;
    for (int i = 0; i < n; i++) {
        freeMen.push(i);
    }
    
    // Gale-Shapley Algorithm
    while (!freeMen.empty()) {
        int man = freeMen.front();
        freeMen.pop();
        
        int woman = menPref[man][nextWomanIndex[man]++];
        // Current partner of this woman
        int currentPartner = womanPartner[woman];
        
        // If woman is free or prefers this new man
        if (currentPartner == -1 ||
            prefersOverCurrent(woman, man, currentPartner, womenPref, n)) {
            womanPartner[woman] = man;
            manPartner[man] = woman;
            if (currentPartner != -1) {
                manPartner[currentPartner] = -1;
                freeMen.push(currentPartner);
            }
        } else {
            // Woman rejects man
            freeMen.push(man);
        }
    }
    
    cout << "Stable Pairs:" << endl;
    for (int m = 0; m < n; m++) {
        cout << "Man " << m << " is paired with Woman " << manPartner[m] << endl;
    }
    
    return 0;
}
