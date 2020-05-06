#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int L;
    cin >> L; cin.ignore();
    int H;
    cin >> H; cin.ignore();
    string T;
    getline(cin, T);
    for (auto& c : T) {
        if (c >= 'A' && c <= 'Z') {
            // OK
        } else if (c >= 'a' && c <= 'z') {
            c += 'A' - 'a';
        } else {
            c = '?';
        }
    }
        
    vector < string > alphabet(H);
    for (auto& item : alphabet) {
        getline(cin, item);    
    }

    vector < string > text(H);
    
    for (const auto& c : T) {
        int symbol = c - 'A';
        if (c == '?') {
            symbol = 'Z' - 'A' + 1;
        }
        
        for (int i = 0; i < H; ++i) {
            text[i] += alphabet[i].substr(symbol*L, L);
        }
    }
    
    for (const auto& line : text) {
        cout << line << endl;    
    }
    
    return 0;
}