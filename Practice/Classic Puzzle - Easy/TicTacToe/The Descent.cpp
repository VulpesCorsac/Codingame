#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * The while loop represents the game.
 * Each iteration represents a turn of the game
 * where you are given inputs (the heights of the mountains)
 * and where you have to print an output (the index of the mountain to fire on)
 * The inputs you are given are automatically updated according to your last actions.
 **/

int main() {

    // game loop
    while (1) {
        int max_height_value = 0;
        int max_height_number = 0;
        for (int i = 0; i < 8; i++) {
            int mountainH; // represents the height of one mountain.
            cin >> mountainH;
            cin.ignore();
            
            if (mountainH > max_height_value) {
                max_height_value = mountainH;
                max_height_number = i;
            }
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        cout << max_height_number << endl; // The index of the mountain to fire on.
    }
    
    return 0;
}