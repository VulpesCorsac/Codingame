#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>

using namespace std;

static auto _ = [] () { ios_base::sync_with_stdio(false); cin.tie(nullptr); return 0; } ();

constexpr bool DEBUG_OUTPUT = false;

struct Node {
    Node* dot  = nullptr;
    Node* dash = nullptr;

    long long words = 0;
};

string encode(const string& str) {
    string result;
    for (auto& c : str) {
        switch (c) {
            case 'A' :
                result += ".-";
                break;
            case 'B' :
                result += "-...";
                break;
            case 'C' :
                result += "-.-.";
                break;
            case 'D' :
                result += "-..";
                break;
            case 'E' :
                result += ".";
                break;
            case 'F' :
                result += "..-.";
                break;
            case 'G' :
                result += "--.";
                break;
            case 'H' :
                result += "....";
                break;
            case 'I' :
                result += "..";
                break;
            case 'J' :
                result += ".---";
                break;
            case 'K' :
                result += "-.-";
                break;
            case 'L' :
                result += ".-..";
                break;
            case 'M' :
                result += "--";
                break;
            case 'N' :
                result += "-.";
                break;
            case 'O' :
                result += "---";
                break;
            case 'P' :
                result += ".--.";
                break;
            case 'Q' :
                result += "--.-";
                break;
            case 'R' :
                result += ".-.";
                break;
            case 'S' :
                result += "...";
                break;
            case 'T' :
                result += "-";
                break;
            case 'U' :
                result += "..-";
                break;
            case 'V' :
                result += "...-";
                break;
            case 'W' :
                result += ".--";
                break;
            case 'X' :
                result += "-..-";
                break;
            case 'Y' :
                result += "-.--";
                break;
            case 'Z' :
                result += "--..";
                break;
            default :
                throw runtime_error("Wrong character");
        }
    }

    return result;
}

void insert(Node* root, const string& str)   {
    for (const auto& c : str) {
        if (c == '.') {
            if (!root->dot) {
                root->dot = new Node;
            }

            root = root->dot;
        } else {
            if (!root->dash) {
                root->dash = new Node;
            }

            root = root->dash;
        }
    }

    root->words += 1;
}

void deleteNode(Node* root) {
    if (root->dot) {
        deleteNode(root->dot);
    }
    if (root->dash) {
        deleteNode(root->dash);
    }

    delete root;
}

unordered_map < size_t, long long > cache;

long long getWords(Node* root, const string& str, size_t offset = 0) {
    long long ans = 0;
    auto node = root;

    while (offset < str.length()) {
        if (str[offset] == '.' && node->dot) {
            node = node->dot;

            if (DEBUG_OUTPUT) {
                cerr << '.';
            }
        } else if (str[offset] == '-' && node->dash) {
            node = node->dash;

            if (DEBUG_OUTPUT) {
                cerr << '-';
            }
        } else {
            break;
        }

        if (node->words) {
            if (offset + 1 == str.length()) {
                ans += node->words;

                if (DEBUG_OUTPUT) {
                    cerr << endl << "Last word with words = " << node->words << endl;
                }
            } else {
                if (cache.count(offset+1)) {
                    if (DEBUG_OUTPUT) {
                        cerr << "Trying for another word concatenate cached" << endl;
                    }

                    ans += node->words * cache[offset+1];
                } else {
                    if (DEBUG_OUTPUT) {
                        cerr << "Trying for another word concatenate not cached" << endl;
                    }
                    
                    auto tmp_ans = getWords(root, str, offset+1);

                    if (tmp_ans) {
                        ans += node->words * tmp_ans;
                        cache[offset+1] = tmp_ans;
                    }
                }
            }
        }

        ++offset;
    }

    return ans;
}

int main() {
    Node* root = new Node;

    string L;
    cin >> L;

    if (DEBUG_OUTPUT) {
        cerr << L << endl;
    }

    int N;
    cin >> N;

    if (DEBUG_OUTPUT) {
        cerr << N << endl;
    }

    for (int i = 0; i < N; i++) {
        string W;
        cin >> W;
        insert(root, encode(W));

        if (DEBUG_OUTPUT) {
            cerr << W << endl;
        }
    }

    cout << getWords(root, L) << endl;

    deleteNode(root);
}

