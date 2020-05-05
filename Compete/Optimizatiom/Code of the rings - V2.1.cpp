// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>

//#define calculateScheme
#define MAIN_DEBUG_OUTPUT
//#define MIDDLE_DEBUG_OUTPUT
//#define MORE_DEBUG_OUTPUT
#define Fast

using namespace std;

constexpr bool generate_answer = true;
constexpr bool read_from_file  = false;

constexpr int alphabet_size = 'Z' - 'A' + 1 + 1;
constexpr int stones = 30;

constexpr int can_dp_default_base = 500;
constexpr int can_dp_smart_base = 45;
constexpr int can_dp_best_base = 20;
constexpr int dp_smart_base_step = 2;

inline int totalLength(const vector < string >& x) {
    int result = 0;
    for (const auto& str : x) {
        result += str.length();
    }
    return result;
}

inline auto concatenate(const vector < string >& x) {
    auto ans = string();
    for (const auto& item : x) {
        ans += item;
    }
    return ans;
}

inline void print(const string& x, bool debug = false) {
    if (debug) {
        cerr << x;
    } else {
        cout << x;
    }
}

inline void print(const vector < string >& x, bool debug = false) {
    for (const auto& str : x) {
        if (debug) {
            cerr << str;
        } else {
                cout << str;
        }
    }
}

inline int getNumFromSymbol(char c) {
    if (c == ' ') {
        return 0;
    } else {
        return c - 'A' + 1;
    }
}

inline char getSymbolFromNum(int i) {
    if (i == 0) {
        return ' ';
    } else {
        return 'A' + i - 1;
    }
}

inline auto getScriptResult(const string& script) {
//    return S;

    auto ans = string("");
    int symbols_on_stones[stones];
    for (int stone = 0; stone < stones; ++stone) {
        symbols_on_stones[stone] = 0;
    }
    int pos = 0;
    int brc = 0;

    for (int i = 0; i < script.size(); ++i) {
        const auto& c = script[i];
        if (c == '.') {
            ans += getSymbolFromNum(symbols_on_stones[pos]);
        } else if (c == '>') {
            pos = (pos + 1 + stones) % stones;
        } else if (c == '<') {
            pos = (pos - 1 + stones) % stones;
        } else if (c == '+') {
            symbols_on_stones[pos] = (symbols_on_stones[pos] + 1 + alphabet_size) % alphabet_size;
        } else if (c == '-') {
            symbols_on_stones[pos] = (symbols_on_stones[pos] - 1 + alphabet_size) % alphabet_size;
        } else if (c == '[') {
            if (!symbols_on_stones[pos]) {
                ++brc;
                while (brc) {
                    ++i;
                    if (script[i] == '[') {
                        ++brc;
                    }
                    if (script[i] == ']') {
                        --brc;
                    }
                }
            } else {
                continue;
            }
        } else if (c == ']') {
            if (!symbols_on_stones[pos]) {
                continue;
            } else {
                if (script[i] == ']') {
                    ++brc;
                }
                while (brc) {
                    --i;
                    if (script[i] == '[') {
                        --brc;
                    }
                    if (script[i] == ']') {
                        ++brc;
                    }
                }
                --i;
            }
        }
    }

    return ans;
}

auto getScriptResult(const vector < string >& script) {
    return getScriptResult(concatenate(script));
}

inline auto getSymbolDistance(char next, char prev, char operation) {
    if (operation == '+') {
        return (getNumFromSymbol(next) - getNumFromSymbol(prev) + alphabet_size) % alphabet_size;
    } else {
        return (getNumFromSymbol(prev) - getNumFromSymbol(next) + alphabet_size) % alphabet_size;
    }
}

inline auto getPositionDistance(int next, int prev, char movement) {
    if (movement == '>') {
        return (next - prev + stones) % stones;
    } else {
        return (prev - next + stones) % stones;
    }
}

inline auto repeatChar(char c, int r) {
    auto ans = string();
    ans.assign(r, c);
    return ans;
}

inline auto inverseCommand(const string& command) {
    auto ans = string(command);

    for (auto& symbol: ans) {
        if (symbol == '+') {
            symbol =  '-';
        } else if (symbol == '-') {
            symbol =  '+';
        }
    }

    return ans;
}

string cache[alphabet_size][alphabet_size];

auto getClosestSymbolInstruction(char next, char prev, bool can_flush) {
    auto ans = string();

    if (next != prev && !cache[getNumFromSymbol(next)][getNumFromSymbol(prev)].empty()) {
        ans = cache[getNumFromSymbol(next)][getNumFromSymbol(prev)];
    } else {
        if (next != prev) {
            auto plain_plus_len  = getSymbolDistance(next, prev, '+');
            auto plain_minus_len = getSymbolDistance(next, prev, '-');

            string plain_min = "";
            if (plain_plus_len <= plain_minus_len) {
                plain_min = std::move(repeatChar('+', plain_plus_len));
            } else {
                plain_min = std::move(repeatChar('-', plain_minus_len));
            }

            auto flush_plus_len  = getSymbolDistance(next, ' ', '+');
            auto flush_minus_len = getSymbolDistance(next, ' ', '-');

            string flush_min = "";
            if (flush_plus_len <= flush_minus_len) {
                flush_min = std::move(repeatChar('+', flush_plus_len));
            } else {
                flush_min = std::move(repeatChar('-', flush_minus_len));
            }

            if (prev != ' ') {
                if (next != ' ') {
                    if (next <= 'M') {
                        flush_min = "[+]" + flush_min;
                    } else {
                        flush_min = "[-]" + flush_min;
                    }
                } else {
                    if (next <= 'M') {
                        flush_min = "[+]";
                    } else {
                        flush_min = "[-]";
                    }
                }
            }

            if (plain_min.length() <= flush_min.length()) {
                ans = std::move(plain_min);
            } else {
                ans = std::move(flush_min);
            }
        }

        cache[getNumFromSymbol(next)][getNumFromSymbol(prev)] = ans;
    }

    return ans;
}

auto getPositionMoveInstruction(int next, int prev) {
    auto ans = string();

    if (next != prev) {
        auto right_len = getPositionDistance(next, prev, '>');
        auto left_len  = getPositionDistance(next, prev, '<');

        if (right_len <= left_len) {
            ans = std::move(repeatChar('>', right_len));
        } else {
            ans = std::move(repeatChar('<', left_len));
        }
    }

    return ans;
}

auto getBaseOnStones(char base, bool can_flush) {
    auto setBase = getClosestSymbolInstruction(base, ' ', can_flush);
    return inverseCommand(setBase) + "[>" + setBase + "]" + setBase;
}

#ifdef calculateScheme
constexpr int max_scheme = 256;
constexpr int max_multiplier = 40;
string scheme[max_scheme][max_scheme];

void fillScheme() {
    for (int x = 0; x < max_scheme; ++x) {
        for (int y = 0; y < max_scheme; ++y) {
            auto d = y - x;
            if (d > +max_scheme / 2) {
                d -= max_scheme;
            }
            if (d < -max_scheme / 2) {
                d += max_scheme;
            }

            if (d >= 0) {
                scheme[x][y] = repeatChar('+', +d);
            } else {
                scheme[x][y] = repeatChar('-', -d);
            }
        }
    }

    bool shorten = true;
    while (shorten) {
        shorten = false;

        for (int x = 0; x < max_scheme; ++x) {
            for (int n = 1; n < max_multiplier; ++n) {
                for (int d = 1; d < max_multiplier; ++d) {
                    int j = x;
                    int y = 0;

                    for (int i = 0; i < max_scheme; ++i) {
                        if (j == 0) {
                            break;
                        }
                        j = (j - d + max_scheme) & (max_scheme - 1);
                        y = (y + n)              & (max_scheme - 1);
                    }

                    if (j == 0) {
                        string tmp = "[" + repeatChar('-', d) + ">" + repeatChar('+', n) + "<]>";

                        if (tmp.length() < scheme[x][y].length()) {
                            scheme[x][y] = std::move(tmp);
                            shorten = true;
                        }
                    }

                    j = x;
                    y = 0;

                    for (int i = 0; i < max_scheme; ++i) {
                        if (j == 0) {
                            break;
                        }
                        j = (j + d)              & (max_scheme - 1);
                        y = (y - n + max_scheme) & (max_scheme - 1);
                    }

                    if (j == 0) {
                        string tmp = "[" + repeatChar('+', d) + ">" + repeatChar('-', n) + "<]>";

                        if (tmp.length() < scheme[x][y].length()) {
                            scheme[x][y] = std::move(tmp);
                            shorten = true;
                        }

                    }
                }
            }
        }

        for (int x = 0; x < max_scheme; ++x) {
            for (int y = 0; y < max_scheme; ++y) {
                for (int z = 0; z < max_scheme; ++z) {
                    if (scheme[x][y].length() > scheme[x][z].length() + scheme[z][y].length()) {
                        scheme[x][y] = scheme[x][z] + scheme[z][y];
                        shorten = true;
                    }
                }
            }
        }
    }

    for (int i = 0; i < alphabet_size; ++i) {
        cerr << "i : " << i << endl << " j : ";
        cout << "{";
        for (int j = 0; j < alphabet_size; ++j) {
            cerr << j << " ";
            cout << "{\"" << scheme[i][j] << "\"}";
            if (j != alphabet_size-1) {
                cout << ",";
            }
        }
        cerr << endl;
        cout << "},\n";
    }
}
#else
vector < vector < string > > scheme = {
    {{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"+++++++++++"},{"++++++++++++"},{"+++++++++++++"},{"++++++++++++++"},{"+++[->+++++<]>"},{"++++[->++++<]>"},{"++++[->++++<]>+"},{"+++[->++++++<]>"},{"+++[->++++++<]>+"},{"++++[->+++++<]>"},{"+++[->+++++++<]>"},{"++++[->+++++<]>++"},{"++++[->++++++<]>-"},{"++++[->++++++<]>"},{"+++++[->+++++<]>"},{"+++++[->+++++<]>+"}},
    {{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"+++++++++++"},{"++++++++++++"},{"+++++++++++++"},{"++[->+++++<]>"},{"+++[->++++<]>"},{"+++[->++++<]>+"},{"++[->++++++<]>"},{"++[->++++++<]>+"},{"+++[->+++++<]>"},{"++[->+++++++<]>"},{"+++[->+++++<]>++"},{"+++[->++++++<]>-"},{"+++[->++++++<]>"},{"++++[->+++++<]>"},{"++++[->+++++<]>+"}},
    {{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"+++++++++++"},{"++++++++++++"},{"+[->+++++<]>"},{"++[->++++<]>"},{"++[->++++<]>+"},{"+[->++++++<]>"},{"+[->++++++<]>+"},{"++[->+++++<]>"},{"+[->+++++++<]>"},{"++[->+++++<]>++"},{"++[->++++++<]>-"},{"++[->++++++<]>"},{"+++[->+++++<]>"},{"+++[->+++++<]>+"}},
    {{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"+++++++++++"},{"[->+++++<]>"},{"+[->++++<]>"},{"+[->++++<]>+"},{"[->++++++<]>"},{"[->++++++<]>+"},{"+[->+++++<]>"},{"[->+++++++<]>"},{"+[->+++++<]>++"},{"+[->++++++<]>-"},{"+[->++++++<]>"},{"++[->+++++<]>"},{"++[->+++++<]>+"}},
    {{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"+[->+++<]>"},{"[->++++<]>"},{"[->++++<]>+"},{"++[->+++<]>"},{"++[->+++<]>+"},{"[->+++++<]>"},{"+++[->+++<]>"},{"[->+++++<]>++"},{"[->++++++<]>-"},{"[->++++++<]>"},{"+[->+++++<]>"},{"+[->+++++<]>+"}},
    {{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"[->+++<]>"},{"[->+++<]>+"},{"[->+++<]>++"},{"+[->+++<]>"},{"+[->+++<]>+"},{"[->++++<]>"},{"++[->+++<]>"},{"[->++++<]>++"},{"+[->++++<]>-"},{"+[->++++<]>"},{"[->+++++<]>"},{"[->+++++<]>+"}},
    {{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"[->+++<]>-"},{"[->+++<]>"},{"[->+++<]>+"},{"-[->++++<]>"},{"+[->+++<]>"},{"+[->+++<]>+"},{"[->++++<]>-"},{"[->++++<]>"},{"[->++++<]>+"},{"[->++++<]>++"}},
    {{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"-[->+++<]>"},{"-[->+++<]>+"},{"[->+++<]>-"},{"[->+++<]>"},{"[->+++<]>+"},{"[->+++<]>++"},{"+[->+++<]>"},{"+[->+++<]>+"},{"+[->+++<]>++"}},
    {{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"+[->++<]>"},{"+[->++<]>+"},{"++[->++<]>"},{"-[->+++<]>"},{"-[->+++<]>+"},{"[->+++<]>-"},{"[->+++<]>"},{"[->+++<]>+"},{"[->+++<]>++"}},
    {{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"[->++<]>"},{"[->++<]>+"},{"+[->++<]>"},{"+[->++<]>+"},{"++[->++<]>"},{"-[->+++<]>-"},{"-[->+++<]>"},{"-[->+++<]>+"},{"[->+++<]>-"}},
    {{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"[->++<]>"},{"[->++<]>+"},{"+[->++<]>"},{"+[->++<]>+"},{"++[->++<]>"},{"++[->++<]>+"},{"-[->+++<]>-"}},
    {{"-----------"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"[->++<]>-"},{"[->++<]>"},{"[->++<]>+"},{"+[->++<]>"},{"+[->++<]>+"},{"++[->++<]>"}},
    {{"------------"},{"-----------"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"-[->++<]>"},{"[->++<]>-"},{"[->++<]>"},{"[->++<]>+"},{"+[->++<]>"}},
    {{"-------------"},{"------------"},{"-----------"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"-[->++<]>"},{"[->++<]>-"},{"[->++<]>"}},
    {{"--------------"},{"-------------"},{"------------"},{"-----------"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"-[->++<]>-"},{"-[->++<]>"}},
    {{"[----->+<]>---"},{"[----->+<]>--"},{"[----->+<]>-"},{"[----->+<]>"},{"[--->+<]>-"},{"[--->+<]>"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"},{"--[->++<]>"}},
    {{"[---->+<]>----"},{"[---->+<]>---"},{"[---->+<]>--"},{"[---->+<]>-"},{"[---->+<]>"},{"-[--->+<]>"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"},{"++++++++++"}},
    {{"-[---->+<]>----"},{"-[---->+<]>---"},{"-[---->+<]>--"},{"-[---->+<]>-"},{"-[---->+<]>"},{"--[--->+<]>"},{"+[--->+<]>"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"},{"+++++++++"}},
    {{"[------>+<]>---"},{"[------>+<]>--"},{"[------>+<]>-"},{"[------>+<]>"},{"[--->+<]>--"},{"[--->+<]>-"},{"[--->+<]>"},{"[--->+<]>+"},{"[-->+<]>-"},{"[-->+<]>"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"},{"++++++++"}},
    {{"-[------>+<]>---"},{"-[------>+<]>--"},{"-[------>+<]>-"},{"-[------>+<]>"},{"-[--->+<]>--"},{"-[--->+<]>-"},{"-[--->+<]>"},{"-[--->+<]>+"},{"-[-->+<]>-"},{"-[-->+<]>"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"},{"+++++++"}},
    {{"[----->+<]>----"},{"[----->+<]>---"},{"[----->+<]>--"},{"[----->+<]>-"},{"[----->+<]>"},{"[---->+<]>"},{"[---->+<]>+"},{"+[--->+<]>"},{"[-->+<]>--"},{"[-->+<]>-"},{"[-->+<]>"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"},{"++++++"}},
    {{"[------->+<]>---"},{"[------->+<]>--"},{"[------->+<]>-"},{"[------->+<]>"},{"[--->+<]>---"},{"[--->+<]>--"},{"[--->+<]>-"},{"[--->+<]>"},{"[--->+<]>+"},{"-[-->+<]>-"},{"-[-->+<]>"},{"+[-->+<]>"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"},{"+++++"}},
    {{"--[----->+<]>----"},{"--[----->+<]>---"},{"--[----->+<]>--"},{"--[----->+<]>-"},{"--[----->+<]>"},{"--[---->+<]>"},{"-[--->+<]>-"},{"-[--->+<]>"},{"-[--->+<]>+"},{"[-->+<]>--"},{"[-->+<]>-"},{"[-->+<]>"},{"[-->+<]>+"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"},{"++++"}},
    {{"+[------>+<]>----"},{"+[------>+<]>---"},{"+[------>+<]>--"},{"+[------>+<]>-"},{"+[------>+<]>"},{"+[---->+<]>-"},{"+[---->+<]>"},{"--[--->+<]>"},{"+[--->+<]>"},{"+[--->+<]>+"},{"-[-->+<]>-"},{"-[-->+<]>"},{"+[-->+<]>"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"},{"+++"}},
    {{"[------>+<]>----"},{"[------>+<]>---"},{"[------>+<]>--"},{"[------>+<]>-"},{"[------>+<]>"},{"[---->+<]>-"},{"[---->+<]>"},{"[--->+<]>-"},{"[--->+<]>"},{"[--->+<]>+"},{"[-->+<]>--"},{"[-->+<]>-"},{"[-->+<]>"},{"[-->+<]>+"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"},{"++"}},
    {{"[----->+<]>-----"},{"[----->+<]>----"},{"[----->+<]>---"},{"[----->+<]>--"},{"[----->+<]>-"},{"[----->+<]>"},{"-[---->+<]>"},{"-[--->+<]>-"},{"-[--->+<]>"},{"-[--->+<]>+"},{"-[-->+<]>--"},{"-[-->+<]>-"},{"-[-->+<]>"},{"+[-->+<]>"},{"+[-->+<]>+"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""},{"+"}},
    {{"-[----->+<]>-----"},{"-[----->+<]>----"},{"-[----->+<]>---"},{"-[----->+<]>--"},{"-[----->+<]>-"},{"-[----->+<]>"},{"--[---->+<]>"},{"--[--->+<]>-"},{"--[--->+<]>"},{"+[--->+<]>"},{"+[--->+<]>+"},{"[-->+<]>--"},{"[-->+<]>-"},{"[-->+<]>"},{"[-->+<]>+"},{"[-->+<]>++"},{"----------"},{"---------"},{"--------"},{"-------"},{"------"},{"-----"},{"----"},{"---"},{"--"},{"-"},{""}}
};
#endif // calculateScheme

inline auto getClosestSymbolInstructionScheme(char next, char prev) {
    return scheme[getNumFromSymbol(next)][getNumFromSymbol(prev)];
}

auto getAnsSingleStone(const string& magic_phrase, bool can_flush) {
    auto ans = vector < string >();

    char prev = ' ';
    for (auto& next : magic_phrase) {
        ans.emplace_back(getClosestSymbolInstruction(next, prev, can_flush));
        ans.emplace_back(".");
        prev = next;
    }

    return ans;
}

auto getAnsBestStone(const string& magic_phrase, char base, bool can_flush) {
    auto ans = vector < string >();

    auto symbols_on_stones = vector < char >(stones, base);
    int bilbo_pos = 0;
    for (auto& symbol : magic_phrase) {
        auto stone_ans = vector < string > (stones);
        for (int stone = 0; stone < stones; ++stone) {
            stone_ans[stone] = std::move(getPositionMoveInstruction(bilbo_pos, stone) + getClosestSymbolInstruction(symbol, symbols_on_stones[stone], can_flush) + ".");
        }

        int best_stone = 0;
        for (int stone = 1; stone < stones; ++stone) {
            if (stone_ans[stone].length() < stone_ans[best_stone].length()) {
                best_stone = stone;
            }
        }

        bilbo_pos = best_stone;
        symbols_on_stones[best_stone] = symbol;
        ans.push_back(std::move(stone_ans[best_stone]));
    }

    return ans;
}

auto getAnsBestStoneBestBase(const string& magic_phrase, bool can_flush) {
    auto ans = getAnsBestStone(magic_phrase, ' ', can_flush);

    for (int i = 1; i < alphabet_size; ++i) {
        auto base = getSymbolFromNum(i);
        auto temp = vector < string >();
        temp.push_back(std::move(getBaseOnStones(base, can_flush)));
        auto temp_symbols = getAnsBestStone(magic_phrase, base, can_flush);
        for (auto& item : temp_symbols) {
            temp.push_back(std::move(item));
        }
        if (totalLength(temp) < totalLength(ans)) {
            ans = std::move(temp);
        }
    }

    return ans;
}

auto getAnsSchemeContinuous(const string& magic_phrase, char base) {
    auto ans = vector < string >();

    auto symbols_on_stones = vector < char >(stones, base);
    int bilbo_pos = 0;
    for (auto symbol : magic_phrase) {
        ans.emplace_back(getClosestSymbolInstructionScheme(symbols_on_stones[bilbo_pos],symbol));
        ans.emplace_back(".");
        ++bilbo_pos;
        bilbo_pos %= stones;
        symbols_on_stones[bilbo_pos] = symbol;
    }

    return ans;
}

auto getAnsSchemeContinuousBestBase(const string& magic_phrase) {
    auto ans = getAnsSchemeContinuous(magic_phrase, ' ');

    for (int i = 1; i < alphabet_size; ++i) {
        auto base = getSymbolFromNum(i);
        auto temp = vector < string >();
        temp.push_back(std::move(getBaseOnStones(base, true)));
        auto temp_symbols = getAnsSchemeContinuous(magic_phrase, base);
        for (auto& item : temp_symbols) {
            temp.push_back(std::move(item));
        }
        if (totalLength(temp) < totalLength(ans)) {
            ans = std::move(temp);
        }
    }

    return ans;
}

auto getAnsBestStoneDP(const string& magic_phrase, char base) {
    vector < string > ans[stones];
    // Which stone I'll take last and what are symbols on stones
    char symbols_on_stones[2][stones][stones];
    for (int i = 0; i < stones; ++i) {
        for (int j = 0; j < stones; ++j) {
            for (int k = 0; k < 2; ++k) {
                symbols_on_stones[k][i][j] = base;
            }
        }
    }

    for (int c = 0; c < magic_phrase.length(); ++c) {
        const auto& symbol = magic_phrase[c];

        #ifdef MIDDLE_DEBUG_OUTPUT
        for (int k = 0; k < 2; ++k) {
            cerr << "K: " << k << endl;
            for (int i = 0; i < stones; ++i) {
                for (int j = 0; j < stones; ++j) {
                    cerr << symbols_on_stones[k][i][j] << "";
                }
                cerr << endl;
            }
        }
        for (int stone = 0; stone < stones; ++stone) {
            cerr << "DP stone " << stone << ": " << totalLength(ans[stone]) << endl;
        }
        cerr << endl;
        #endif // MIDDLE_DEBUG_OUTPUT

        if (c == 0) {
            auto brute = getClosestSymbolInstruction(symbol, base, true);
            auto smart = brute;
            auto best = std::move(smart);

            if (brute.length() <= smart.length()) {
                best = std::move(brute);
            }

            for (int next = 0; next < stones; ++next) {
                ans[next].emplace_back(getPositionMoveInstruction(next, 0));
                ans[next].emplace_back(brute);
                ans[next].emplace_back(".");
                symbols_on_stones[0][next][next] = symbol;
            }
        } else {
            auto id_old = (c - 1) % 2;
            auto id_new = (c - 0) % 2;

            for (int next = 0; next < stones; ++next) {
                #ifdef MORE_DEBUG_OUTPUT
                cerr << "For possibly last stone : " << next << endl;
                #endif // MORE_DEBUG_OUTPUT
                vector < string > tmp[stones];
                #ifdef MIDDLE_DEBUG_OUTPUT
                int tmp_len[stones];
                for (int i = 0; i < stones; ++i) {
                    tmp_len[i] = 0;
                }
                #endif // MIDDLE_DEBUG_OUTPUT
                for (int prev = 0; prev < stones; ++prev) {
                    #ifdef MORE_DEBUG_OUTPUT
                    cerr << "For prev = " << prev << endl;
                    cerr << "Position command: " << getPositionMoveInstruction(next, prev) << endl;
                    cerr << "Changing symbol from: " << symbols_on_stones[id_old][prev][next] << " to " << symbol << endl;
                    cerr << "Symbol command: " << getClosestSymbolInstruction(symbol, symbols_on_stones[id_old][prev][next], true) << endl;
                    #endif // MORE_DEBUG_OUTPUT
                    tmp[prev].emplace_back(getPositionMoveInstruction(next, prev));
                    tmp[prev].emplace_back(getClosestSymbolInstruction(symbol, symbols_on_stones[id_old][prev][next], true));
                    tmp[prev].emplace_back(".");
                    #ifdef MIDDLE_DEBUG_OUTPUT
                    tmp_len[prev] = totalLength({getPositionMoveInstruction(next, prev), getClosestSymbolInstruction(symbol, symbols_on_stones[id_old][prev][next], true), "."});
                    #endif // MIDDLE_DEBUG_OUTPUT
                }

                int best_dp = 0;
                for (int stone = 1; stone < stones; ++stone) {
                    if (totalLength(ans[stone  ]) + totalLength(tmp[stone  ]) <
                        totalLength(ans[best_dp]) + totalLength(tmp[best_dp])) {
                        best_dp = stone;
                    }
                }

                #ifdef MORE_DEBUG_OUTPUT
                cerr << "Best stone : " << best_dp << endl;
                #endif // MORE_DEBUG_OUTPUT

                for (int i = 0; i < stones; ++i) {
                    symbols_on_stones[id_new][next][i] = symbols_on_stones[id_old][best_dp][i];
                }

                symbols_on_stones[id_new][next][next] = symbol;

                if (next != best_dp) {
                    ans[next] = ans[best_dp];
                }
                for (auto& item : tmp[best_dp]) {
                    #ifdef MORE_DEBUG_OUTPUT
                    cerr << "move : " << item << endl;
                    #endif // MORE_DEBUG_OUTPUT
                    ans[next].push_back(std::move(item));
                }
            }

            #ifdef MORE_DEBUG_OUTPUT
            for (int stone = 0; stone < stones; ++stone) {
                cerr << "Stone: " << stone << " ";
                cerr << getScriptResult(ans[stone]) << " ";
                print(ans[stone], true);
                cerr << endl;
            }
            #endif // MORE_DEBUG_OUTPUT
        }
    }

    #ifdef MIDDLE_DEBUG_OUTPUT
    for (int k = 0; k < 2; ++k) {
        cerr << "K: " << k << endl;
        for (int i = 0; i < stones; ++i) {
            for (int j = 0; j < stones; ++j) {
                cerr << symbols_on_stones[k][i][j] << "";
            }
            cerr << endl;
        }
    }
    for (int stone = 0; stone < stones; ++stone) {
        cerr << "DP stone " << stone << ": " << totalLength(ans[stone]) << endl;
    }
    cerr << endl;
    #endif // MIDDLE_DEBUG_OUTPUT

    int best = 0;
    for (int stone = 1; stone < stones; ++stone) {
        if (totalLength(ans[stone]) <= totalLength(ans[best])) {
            best = stone;
        }
    }

    return ans[best];
}

auto getAnsBestStoneSmartBaseDP(const string& magic_phrase) {
    set < int > best_bases;
    int positive_base = 0;
    int negative_base = 0;
    for (const auto& c : magic_phrase) {
        auto num = getNumFromSymbol(c);
        positive_base += num;
        if (c <= 'M') {
            negative_base += num;
        } else {
            negative_base -= getSymbolDistance(c, ' ', '-');
        }
    }
    while (negative_base < 0) {
        negative_base += alphabet_size;
    }
    positive_base /= magic_phrase.length();
    positive_base %= alphabet_size;
    negative_base /= magic_phrase.length();
    negative_base %= alphabet_size;
    for (int step = -dp_smart_base_step; step <= dp_smart_base_step; ++step) {
        best_bases.insert((positive_base + step + alphabet_size) % alphabet_size);
        best_bases.insert((negative_base + step + alphabet_size) % alphabet_size);
    }

    auto ans = getAnsBestStoneDP(magic_phrase, ' ');
    for (const auto& base : best_bases) {
        if (base == 0) {
            continue;
        }
        auto temp = vector < string >();
        temp.push_back(std::move(getBaseOnStones(getSymbolFromNum(base), true)));
        auto temp_symbols = getAnsBestStoneDP(magic_phrase, getSymbolFromNum(base));
        for (auto& item : temp_symbols) {
            temp.push_back(std::move(item));
        }
        if (totalLength(temp) < totalLength(ans)) {
            ans = std::move(temp);
        }
    }

    return ans;
}

auto getAnsBestStoneBestBaseDP(const string& magic_phrase) {
    auto ans = getAnsBestStoneDP(magic_phrase, ' ');

    for (int i = 1; i < alphabet_size; ++i) {
        auto base = getSymbolFromNum(i);
        auto temp = vector < string >();
        temp.push_back(std::move(getBaseOnStones(base, true)));
        auto temp_symbols = getAnsBestStoneDP(magic_phrase, base);
        for (auto& item : temp_symbols) {
            temp.push_back(std::move(item));
        }
        if (totalLength(temp) < totalLength(ans)) {
            ans = std::move(temp);
        }
    }

    return ans;
}

auto getAns(const string& magic_phrase) {
    #ifndef Fast
    auto best = getAnsSingleStone(magic_phrase, true);
    auto temp = vector < string >();
    temp = std::move(getAnsBestStone(magic_phrase, ' ', false));
    if (totalLength(temp) < totalLength(best)) {
        cerr << "Best stone without flush is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
        best = std::move(temp);
    }
    temp = std::move(getAnsBestStone(magic_phrase, ' ', true));
    if (totalLength(temp) < totalLength(best)) {
        cerr << "Best stone with flush is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
        best = std::move(temp);
    }
    temp = std::move(getAnsBestStoneBestBase(magic_phrase, false));
    if (totalLength(temp) < totalLength(best)) {
        cerr << "Best stone best base without flush is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
        best = std::move(temp);
    }
    temp = std::move(getAnsBestStoneBestBase(magic_phrase, true));
    if (totalLength(temp) < totalLength(best)) {
        cerr << "Best stone best base with flush is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
        best = std::move(temp);
    }
    temp = std::move(getAnsSchemeContinuous(magic_phrase, ' '));
    if (totalLength(temp) < totalLength(best)) {
        cerr << "Scheme continuous is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
        best = std::move(temp);
    }
    temp = std::move(getAnsSchemeContinuousBestBase(magic_phrase));
    if (totalLength(temp) < totalLength(best)) {
        cerr << "Scheme continuous best base is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
        best = std::move(temp);
    }
    temp = std::move(getAnsBestStoneDP(magic_phrase, ' '));
    if (totalLength(temp) < totalLength(best)) {
        if (getScriptResult(temp) == magic_phrase) {
            cerr << "DP is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
            best = std::move(temp);
        } else {
            cerr << "DP might be better but failed" << endl;
            print(temp, true);
            cerr << endl;
            cerr << getScriptResult(temp) << endl;
        }
    }
    temp = std::move(getAnsBestStoneSmartBaseDP(magic_phrase));
    if (totalLength(temp) < totalLength(best)) {
        cerr << "Smart base DP is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
        best = std::move(temp);
    }
    temp = std::move(getAnsBestStoneBestBaseDP(magic_phrase));
    if (totalLength(temp) < totalLength(best)) {
        cerr << "Best base DP is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
        best = std::move(temp);
    }
    #else
    auto best = getAnsBestStoneBestBase(magic_phrase, true);
    auto temp = vector < string >();
    temp = std::move(getAnsSchemeContinuousBestBase(magic_phrase));
    if (totalLength(temp) < totalLength(best)) {
        cerr << "Scheme continuous best base is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
        best = std::move(temp);
    }
    bool dp = false;
    if (magic_phrase.length() < can_dp_best_base) {
        dp = true;
        cerr << "Starting DP best base" << endl;
        temp = std::move(getAnsBestStoneBestBaseDP(magic_phrase));
        if (totalLength(temp) < totalLength(best)) {
            cerr << "Best base DP is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
            best = std::move(temp);
        }
    }
    if (!dp && magic_phrase.length() < can_dp_smart_base) {
        dp = true;
        cerr << "Starting DP smart base" << endl;
        temp = std::move(getAnsBestStoneSmartBaseDP(magic_phrase));
        if (totalLength(temp) < totalLength(best)) {
            cerr << "Smart base DP is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
            best = std::move(temp);
        }
    }
    if (!dp && magic_phrase.length() > can_dp_default_base) {
        dp = true;
        cerr << "Starting DP default base" << endl;
        temp = std::move(getAnsBestStoneDP(magic_phrase, ' '));
        if (totalLength(temp) < totalLength(best)) {
            cerr << "DP is better : " << totalLength(temp) << " vs " << totalLength(best) << endl;
            best = std::move(temp);
        }
    }
    #endif // Fast

    return best;
}

int main() {
//    cerr << getScriptResult("-.++.<.-.>.") << endl;
    #ifdef calculateScheme
    freopen("input.txt", "r+", stdin);
    freopen("output.txt", "w+", stdout);
    fillScheme();
    #else
    if (read_from_file) {
        freopen("input.txt", "r+", stdin);
        freopen("output.txt", "w+", stdout);

        string magicPhrase;
        while (getline(cin, magicPhrase)) {
            if (magicPhrase == "#") {
                break;
            }
            if (!magicPhrase.empty()) {
                #ifdef MAIN_DEBUG_OUTPUT
                cerr << magicPhrase << endl;
                #endif // MAIN_DEBUG_OUTPUT
                cout << "{\"" << magicPhrase << "\",\"";
                print(getAns(magicPhrase));
                cout << "\"}," << endl;
            }
        }
    } else {
        string magicPhrase;
        getline(cin, magicPhrase);
        print(getAns(magicPhrase));
        cout << endl;
    }
    #endif // calculateScheme
}
