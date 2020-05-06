#include <iostream>
#include <string>
#include <bitset>

using namespace std;

string bitConvert(const string& str) {
    string ans;
    
    for (const auto& c : str)
        ans += bitset < 7 >(c).to_string();
    
    return ans;
}

string norrisConvert (const string& str) {
    string output = "";
    string temp = "";
    int isZero = -1;
    
    for (const auto& c : str) {
        int newIsZero = (c == '0');
   
        if (newIsZero == isZero) {
            temp += "0";
        } else {
            isZero = newIsZero;
            
            if (output.length())
                output += " ";
            
            output += temp;
            
            temp = isZero ? "00 0" : "0 0";
        }
    }
    
    if (temp.length()) {
        if (output.length())
            output += " ";
        output += temp;
    }

    return output;
}

int main() {
    string message;
    getline(cin, message);
    
    cout << norrisConvert(bitConvert(message)) << endl;
}
