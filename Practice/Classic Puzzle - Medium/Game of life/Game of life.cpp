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
    int width;
    int height;
    cin >> width >> height;
    vector < vector < int > > matrix (height, vector < int > (width));
    for (int i = 0; i < height; i++) {
        string line;
        cin >> line;
        for (int j = 0; j < width; ++j) {
            matrix[i][j] = line[j] - '0';   
        }
    }

    auto ans = matrix;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int alive_neighbors = -matrix[i][j];
            
            for (int di = -1; di <= +1; ++di) {
                for (int dj = -1; dj <= +1; ++dj) {
                    if (i + di >= 0 && 
                        i + di < height &&
                        j + dj >= 0 && 
                        j + dj < width && 
                        matrix[i+di][j+dj]) {
                        ++alive_neighbors;
                    }
                }
            }
            
            /*   
            cerr << "{" << i << "," << j << "}=" << matrix[i][j] << "->";
            cerr << alive_neighbors << endl;
            //*/
            
            if (alive_neighbors < 2 || alive_neighbors > 3) {
                ans[i][j] = 0;   
            } else if (!matrix[i][j] && alive_neighbors == 3) {
                ans[i][j] = 1;
            }
        }
    }
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cout << ans[i][j];   
        }
        cout << endl;
    }
}