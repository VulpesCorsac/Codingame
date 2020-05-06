#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector < string > board;

bool canWin() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == '.') {
                if (board[i][(j+1)%3] == 'O' && board[i][(j+2)%3] == 'O') {
                    board[i][j] = 'O';
                    return true;
                }
                if (board[(i+1)%3][j] == 'O' && board[(i+2)%3][j] == 'O') {
                    board[i][j] = 'O';
                    return true;
                }
                if (i == j) {
                    if (board[(i+1)%3][(j+1)%3] == 'O' && board[(i+2)%3][(j+2)%3] == 'O') {
                        board[i][j] = 'O';
                        return true;
                    }
                }
                if (i+j == 2) {
                    if (board[(i+1)%3][(j+2)%3] == 'O' && board[(i+2)%3][(j+1)%3] == 'O') {
                        board[i][j] = 'O';
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    board.resize(3);
    for (auto& line : board) {
        cin >> line;
    }

    if (canWin()) {
        for (const auto& line : board) {
            cout << line << endl;
        }
    } else {
        cout << "false";
    }
}