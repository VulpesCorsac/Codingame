#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 * ---
 * Hint: You can use the debug stream to print initialTX and initialTY, if Thor seems not follow your orders.
 **/

int main() {
    int lightX; // the X position of the light of power
    int lightY; // the Y position of the light of power
    int initialTX; // Thor's starting X position
    int initialTY; // Thor's starting Y position
    cin >> lightX >> lightY >> initialTX >> initialTY; 
    cin.ignore();

    // game loop
    while (1) {
        int remainingTurns; // The remaining amount of turns Thor can move. Do not remove this line.
        cin >> remainingTurns;
        cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        
        string direction;
        if (initialTX > lightX) {
            initialTX -= 1;
            if (initialTY > lightY) {
                direction = "NW";
                initialTY -= 1;
            } else if (initialTY < lightY) {
                direction = "SW";
                initialTY += 1;
            } else {
                direction = "W";
            }
        } else if (initialTX < lightX) {
            initialTX += 1;
            if (initialTY > lightY) {
                direction = "NE";
                initialTY -= 1;
            } else if (initialTY < lightY) {
                direction = "SE";
                initialTY += 1;
            } else {
                direction = "E";
            }
        } else {
            if (initialTY > lightY) {
                direction = "N";
                initialTY -= 1;
            } else if (initialTY < lightY) {
                direction = "S";
                initialTY += 1;
            } else {
                direction = "";                
            }
        }
        
        // A single line providing the move to be made: N NE E SE S SW W or NW
        cout << direction << endl;
    }
}