#include <iostream>

using namespace std;

int main() {
    int a, b, n;
    cin >> a >> b; cin.ignore();
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x; cin.ignore();
        cout << a *x + b << endl;
    }
}