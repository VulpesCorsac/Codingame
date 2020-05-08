#include <iostream>

using namespace std;

int main()
{
    int n;
    bool first = true;
    cin >> n;
    cerr << n << endl;
    while (n >= 1) {
        if (first) {
            cout << n;
            first = false;
        } else {
            cout << " " << n;
        }
        cout.flush();
        if (n == 1) {
            break;
        }
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = n*3 + 1;
        }
    }
}