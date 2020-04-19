#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main() {
    int n; // the number of temperatures to analyse
    cin >> n; 
    cin.ignore();
    int closest = 10000;
    for (int i = 0; i < n; i++) {
        int t; // a temperature expressed as an integer ranging from -273 to 5526
        cin >> t; 
        cin.ignore();
        
        if ((abs(t) < abs(closest)) || (closest == -t && t > 0)) {
            closest = t;
        }
    }
    
    if (closest == 10000) {
        closest = 0;    
    }
    
    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << closest << endl;
}