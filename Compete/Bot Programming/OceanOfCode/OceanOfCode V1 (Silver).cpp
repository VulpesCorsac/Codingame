// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <assert.h>

using namespace std;

static auto _ = [] () { ios_base::sync_with_stdio(false); cin.tie(nullptr); return 0; } ();

typedef pair < int, int > Tile;

struct TileHash {
    inline std::size_t operator()(const Tile& tile) const {
        return tile.first * 31 + tile.second;
    }
};

void lTrim(string& source);
void rTrim(string& source);
void  trim(string& source);
vector < string > splitString(const string& source, char split_symbol);

class Game {
public:
    void readMapData() {
        cin >> width >> height >> my_id;
        assert(height == 15);
        game_map.resize(height);

        for (int row = 0; row < height; row++) {
            string line;
            cin >> line;
            assert(static_cast < int >(line.length()) == width);
            game_map[row].resize(width);

            for (int column = 0; column < width; ++column) {
                if (line[column] == '.') {
                    game_map[row][column] = TILE_FREE;
                    opponent_positions.emplace_back(Tile{row, column});
                } else {
                    game_map[row][column] = TILE_ISLAND;
                }
            }
        }
    }

    void readState() {
        int x, y;
        cin >> x >> y;
        int new_life;
        cin >> new_life;
        if ((!my_state.surfaced && my_state.life + 0 != new_life) ||
            ( my_state.surfaced && my_state.life - 1 != new_life)) {
            my_state.hit = true;

            if (DEBUG_OUTPUT) {
                cerr << "HIT!" << endl;
            }
        } else {
            my_state.hit = false;
        }

        my_state.life = new_life;
        cin >> opponent_state.life;
        cin >> my_state.torpedo_cooldown;
        cin >> my_state.sonar_cooldown;
        cin >> my_state.silence_cooldown;
        cin >> my_state.mine_cooldown;
        cin.ignore();
        string sonar_result;
        cin >> sonar_result;
        cin.ignore();
        string opponent_orders;
        getline(cin, opponent_orders);

        if (DEBUG_OUTPUT) {
//            cerr << "State read" << endl;
        }

        parseOpponentOrders(opponent_orders);
    }

    void updatePossibleOpponentsPositions(const string& opponent_order) {
        if (DEBUG_OUTPUT) {
            cerr << "Parsing order: " << opponent_order << endl;
        }

        vector < string > order = splitString(opponent_order, ' ');
        stringstream out;

        if (order[0] == "move") {
            if (DEBUG_OUTPUT) {
                out << "Move ";
            }

            int direction = -1;

            if (order[1] == "n") {
                if (DEBUG_OUTPUT) {
                    out << "N";
                }

                direction = 0;
            } else if (order[1] == "s") {
                if (DEBUG_OUTPUT) {
                    out << "S";
                }

                direction = 1;
            } else if (order[1] == "w") {
                if (DEBUG_OUTPUT) {
                    out << "W";
                }

                direction = 2;
            } else if (order[1] == "e") {
                if (DEBUG_OUTPUT) {
                    out << "E";
                }

                direction = 3;
            } else {
                if (DEBUG_OUTPUT) {
                    out << "unknown move: " << order[1];
                }
            }

            if (direction >= 0) {
                for (auto& position : opponent_positions) {
                    position.first  += d_row[direction];
                    position.second += d_col[direction];
                }
            }

            auto it = partition(opponent_positions.begin(),
                                opponent_positions.end(),
                                [this](Tile& tile) {
                                    return isInsideMap(tile) && !isIsland(tile);
                                });
            opponent_positions.resize(it - opponent_positions.begin());

            if (DEBUG_OUTPUT) {
                out << "\nPositions: " << opponent_positions.size() << "\n";
            }
        } else if (order[0] == "torpedo") {
            if (DEBUG_OUTPUT) {
                out << "Torpedo";
            }

            int x = atoi(order[1].c_str());
            int y = atoi(order[2].c_str());

            if (DEBUG_OUTPUT) {
                out << " at {" << x << "," << y << "}";
            }

            auto it = partition(opponent_positions.begin(),
                                opponent_positions.end(),
                                [this, x, y](Tile& tile) {
                                    return distance(tile, {y, x}) <= 4;
                                });
            opponent_positions.resize(it - opponent_positions.begin());

            if (DEBUG_OUTPUT) {
                out << "\nPositions: " << opponent_positions.size() << "\n";
            }
        } else if (order[0] == "surface") {
            if (DEBUG_OUTPUT) {
                out << "Surface";
            }

            int sector = atoi(order[1].c_str());

            if (DEBUG_OUTPUT) {
                out << " at sector {" << sector << "}";
            }

            auto it = partition(opponent_positions.begin(),
                                opponent_positions.end(),
                                [this, sector](Tile& tile) {
                                    return isInSector(tile, sector);
                                });
            opponent_positions.resize(it - opponent_positions.begin());

            if (DEBUG_OUTPUT) {
                out << "\nPositions: " << opponent_positions.size() << "\n";
            }
        } else if (order[0] == "sonar") {
            int sector = atoi(order[1].c_str());

            if (DEBUG_OUTPUT) {
                out << " Sonar scanned region " << sector << "\n";
            }
        } else if (order[0] == "silence") {
            if (DEBUG_OUTPUT) {
                out << "Silence";
            }

            unordered_set < Tile , TileHash > new_opponent_positions;

            for (const auto& position : opponent_positions) {
                new_opponent_positions.insert(position);
                for (int direction = 0; direction < directions; ++direction) {
                    for (int length = 1; length <= 4; ++length) {
                        int new_row = position.first  + d_row[direction] * length;
                        int new_col = position.second + d_col[direction] * length;

                        if (isInsideMap(new_row, new_col) &&
                            !isIsland(new_row, new_col) &&
                            canMove(position, new_row, new_col)) {
                            new_opponent_positions.insert({new_row, new_col});
                        }
                    }
                }
            }

            opponent_positions = vector < Tile >(new_opponent_positions.begin(), new_opponent_positions.end());

            auto it = partition(opponent_positions.begin(),
                                opponent_positions.end(),
                                [this](Tile& tile) {
                                    return isInsideMap(tile) && !isIsland(tile);
                                });

            opponent_positions.resize(it - opponent_positions.begin());

            if (DEBUG_OUTPUT) {
                out << "\nPositions: " << opponent_positions.size() << "\n";
            }
        } else if (order[0] == "na") {
            if (DEBUG_OUTPUT) {
                out << "No action\n";
            }
        } else {
            if (DEBUG_OUTPUT) {
                out << "Unknown action: " << order[0] << "\n";
            }
        }

        if (DEBUG_OUTPUT) {
            cerr << out.str() << endl;
        }
    }

    void parseOpponentOrders(string& opponent_orders) {
        if (DEBUG_OUTPUT) {
//            cerr << "Parsing opponent orders" << endl;
        }

        transform(opponent_orders.begin(), opponent_orders.end(), opponent_orders.begin(), ::tolower);

        for (auto& symbol : opponent_orders) {
            if (symbol == '|') {
                symbol = '&';
            }
        }

        vector < string > orders = splitString(opponent_orders, '&');

        if (DEBUG_OUTPUT) {
//            cerr << "Opponent had " << orders.size() << " orders" << endl;
        }

        for (const auto& order : orders) {
            updatePossibleOpponentsPositions(order);
        }

        if (DEBUG_OUTPUT) {
            if (opponent_positions.size() == 1) {
                cerr << "Opponent position at {" << opponent_positions[0].first << "," << opponent_positions[0].second << "}" << endl;
            }
        }
    }

    void makeMove(int direction, const string& charge) {
        insertMyPositionInVisitedTiles();
        cout << "MOVE " << directions_names[direction] << " " << charge;
        decreaseCharged(charge);
    }

    void setMine(const Tile& tile);
    void setMine(int row, int col) {
        if (DEBUG_OUTPUT) {
            cerr << "Mine inserted at {" << row << "," << col << "}" << endl;
        }

        mines.insert({row, col});

        char direction = findDirection(my_state.current_potision, row, col);
        cout << " | MINE " << direction;

        ++current_mines_before_silence;
    }

    bool triggerMine() {
        for (auto& mine : mines) {
            int hit = 0;
            for (const auto& position : opponent_positions) {
                if (distance(mine, position) <= 2) {
                    ++hit;
                }
            }

            if ((hit && opponent_positions.size() < 2) ||
                (hit >= opponent_positions.size() / part_to_trigger_mine)) {
                cout << " | TRIGGER " << mine.second << " " << mine.first;

                mines.erase(mine);

                return true;
            }
        }

        return false;
    }

    void mineActions() {
        if (DEBUG_OUTPUT) {
            cerr << "Mine action" << endl;
        }

        if (my_state.mine_cooldown == 0) {
            random_shuffle(directions_numbers.begin(), directions_numbers.end());
            for (const auto& direction : directions_numbers) {
                int row = my_state.current_potision.first  + d_row[direction];
                int col = my_state.current_potision.second + d_col[direction];

                if (canPlaceMine(row, col)) {
                    setMine(row, col);
                    break;
                }
            }
        }

        triggerMine();
    }

    void surfaceMove();

    void randomMove(const string& charge) {
        if (DEBUG_OUTPUT) {
            cerr << "Random move" << endl;
        }

        int found_direction = -1;
        random_shuffle(directions_numbers.begin(), directions_numbers.end());
        for (const auto& current_direction : directions_numbers) {
            int new_row = my_state.current_potision.first  + d_row[current_direction];
            int new_col = my_state.current_potision.second + d_col[current_direction];
            if ( isInsideMap(new_row, new_col) &&
                !isIsland   (new_row, new_col) &&
                !isVisited  (new_row, new_col)) {
                found_direction = current_direction;
                break;
            }
        }

        if (found_direction >= 0) {
            my_state.current_potision.first  += d_row[found_direction];
            my_state.current_potision.second += d_col[found_direction];
            makeMove(found_direction, charge);
        } else {
            surfaceMove();
        }
    }

    int dfs(const Tile& tile, int depth = 0);
    int dfs(int row, int col, int depth = 0) {
        if (DEBUG_OUTPUT) {
//            cerr << "DFS from {" << row << "," << col << "}" << endl;
        }

        if (!isInsideMap(row, col) ||
            isIsland(row, col) ||
            isVisited(row, col)) {

            return -1;
        }

        if (depth > max_dfs_depth_for_longest_path) {
            return 0;
        }

        visited_tiles.insert({row, col});

        int ans = -1;
        random_shuffle(directions_numbers.begin(), directions_numbers.end());
        for (const auto& current_direction : directions_numbers) {
            int new_row = row + d_row[current_direction];
            int new_col = col + d_col[current_direction];

            ans = max(ans, dfs(new_row, new_col, depth+1));

            if (ans > max_dfs_depth_for_longest_path - depth - 1) {
                break;
            }
        }

        visited_tiles.erase({row, col});

        return ans + 1;
    }

    void longestMove(const string& charge) {
        if (DEBUG_OUTPUT) {
            cerr << "Longest move" << endl;
        }

        int max_length = -1;
        int max_length_direction = -1;
        auto tmp_directions_numbers = directions_numbers; /// WTF?!?! But without it does not work!
        random_shuffle(tmp_directions_numbers.begin(), tmp_directions_numbers.end());
//        printDirectionNumbers();
        for (auto& current_direction : tmp_directions_numbers) {
            int new_row = my_state.current_potision.first  + d_row[current_direction];
            int new_col = my_state.current_potision.second + d_col[current_direction];
            int current_length = dfs(new_row, new_col);

            if (DEBUG_OUTPUT) {
//                cerr << "Direction : " << current_direction << "-" << directions_names[current_direction] << ", length = " << current_length << endl;
            }

            if (current_length > max_length) {
                max_length = current_length;
                max_length_direction = current_direction;

                if (current_length >= max_dfs_depth_for_longest_path) {
                    break;
                }
            }
        }

        if (max_length_direction >= 0) {
            if (DEBUG_OUTPUT) {
                cerr << "Moving to direction number : " << max_length_direction << endl;
            }

            my_state.current_potision.first  += d_row[max_length_direction];
            my_state.current_potision.second += d_col[max_length_direction];
            makeMove(max_length_direction, charge);
        } else {
            surfaceMove();
        }
    }

    void stalkerMove(const Tile& stalk, const string& charge) {
        if (DEBUG_OUTPUT) {
            cerr << "Stalker move" << endl;
        }

        vector < vector < int > > dfs_state(height, vector < int > (width, -1));
        dfs_state[my_state.current_potision.first][my_state.current_potision.second] = 0;

        queue < Tile > dfs;
        dfs.push(my_state.current_potision);

        while (!dfs.empty()) {
            int row = dfs.front().first;
            int col = dfs.front().second;

            if (DEBUG_OUTPUT) {
//                cerr << "Dfs position: {" << row << "," << col << "}\n";
            }

            dfs.pop();

            for (int direction = 0; direction < directions; ++direction) {
                int new_row = row + d_row[direction];
                int new_col = col + d_col[direction];

                if (isInsideMap(new_row, new_col) &&
                    ! isIsland (new_row, new_col) &&
                    !isVisited (new_row, new_col) &&
                     dfs_state [new_row][new_col] == -1) {
                    dfs_state[new_row][new_col] = dfs_state[row][col] + 1;
                    dfs.push({new_row, new_col});
                }

                if (new_row == stalk.first &&
                    new_col == stalk.second) {
                    break;
                }
            }
        }

        /*
        if(DEBUG_OUTPUT) {
            for (int row = 0; row < height; ++row) {
                for (int col = 0; col < width; ++col) {
                    cerr << dfs_state[row][col] << " ";
                }
                cerr << endl;
            }
        }
        //*/

        if (dfs_state[stalk.first][stalk.second] == -1) {
            if (my_state.life >= do_not_surface_when_stalk_below_life) {
                surfaceMove();
            } else {
                longestMove(charge);
            }
        } else {
            int prev_row = stalk.first;
            int prev_col = stalk.second;
            int new_row = prev_row;
            int new_col = prev_col;

            if (DEBUG_OUTPUT) {
//                cerr << "My position: {" << my_state.current_potision.first << "," << my_state.current_potision.second << "}\n";
            }

            while (new_row != my_state.current_potision.first ||
                   new_col != my_state.current_potision.second) {
                if (DEBUG_OUTPUT) {
//                    cerr << "BackTrack position: {" << new_row << "," << new_col << "} = " << dfs_state[prev_row][prev_col] << "\n";
                }

                prev_row = new_row;
                prev_col = new_col;
                for (int direction = 0; direction < directions; ++direction) {
                    new_row = prev_row + d_row[direction];
                    new_col = prev_col + d_col[direction];

                    if (isInsideMap(new_row, new_col) &&
                         dfs_state [new_row][new_col]+1 == dfs_state[prev_row][prev_col]) {
                        if (DEBUG_OUTPUT) {
//                            cerr << "Direction: " << directions_names[direction] << endl;
                        }
                        break;
                    }
                }
            }
            if (DEBUG_OUTPUT) {
//                cerr << "My position: {" << my_state.current_potision.first << "," << my_state.current_potision.second << "}\n";
//                cerr << "My next cell: {" << prev_row << "," << prev_col << "}\n";
            }

            int direction = directionNumber(findDirection(my_state.current_potision, prev_row, prev_col));
            my_state.current_potision.first  += d_row[direction];
            my_state.current_potision.second += d_col[direction];
            makeMove(direction, charge);
        }
    }

    void silentMove() {
        if (DEBUG_OUTPUT) {
            cerr << "Silent move" << endl;
        }

        my_state.must_move_silently = false;

        bool can_move_silently = false;
        int found_direction = -1;
        int found_length = 5;

        for (int length = 4; length > 0; --length) {
            if (DEBUG_OUTPUT) {
//                cerr << "Trying length: " << length << endl;
            }
            for (int direction = 0; direction < directions; ++direction) {
                if (DEBUG_OUTPUT) {
//                    cerr << "Trying direction: " << directions_names[direction] << endl;
                }
                can_move_silently = true;
                for (int step = 1; step <= length; ++step) {
                    int new_row = my_state.current_potision.first  + step * d_row[direction];
                    int new_col = my_state.current_potision.second + step * d_col[direction];
                    if (DEBUG_OUTPUT) {
//                        cerr << "{" << new_row << "," << new_col << "} ";
                    }

                    if (!isInsideMap(new_row, new_col) ||
                         isVisited  (new_row, new_col) ||
                         isIsland   (new_row, new_col)) {
                        if (DEBUG_OUTPUT) {
//                            cerr << " - failed" << endl;
                        }

                        can_move_silently = false;
                        break;
                    }
                }
                if (can_move_silently) {
                    found_direction = direction;
                    found_length = length;
                    break;
                }
            }
            if (can_move_silently) {
                break;
            }
        }

        if (found_direction == -1) {
            surfaceMove();

            my_state.must_move_silently = true;
        } else {
            for (int step = 1; step <= found_length; ++step) {
                visited_tiles.insert({my_state.current_potision.first  + step * d_row[found_direction],
                                      my_state.current_potision.second + step * d_col[found_direction]});
            }
            current_mines_before_silence = 0;

            my_state.current_potision.first  += found_length * d_row[found_direction];
            my_state.current_potision.second += found_length * d_col[found_direction];
            insertMyPositionInVisitedTiles();

            cout << "SILENCE " << directions_names[found_direction] << " " << found_length;
        }
    }

    void shootMove(const string& charge) {
        if (DEBUG_OUTPUT) {
            cerr << "Shoot move" << endl;
        }

        if ((my_state.silence_cooldown == 0) && (my_state.hit || my_state.must_move_silently)) {
            if (DEBUG_OUTPUT) {
                cerr << "Branch 1" << endl;
            }

            longestMove(charge);
            cout << " | ";
            silentMove();
        } else if (opponent_positions.size() > stalk_when_positions_less_than) {
            if (DEBUG_OUTPUT) {
                cerr << "Branch 2" << endl;
            }

            if (current_mines_before_silence >= mines_before_silence && my_state.silence_cooldown == 0) {
                silentMove();
            } else {
                longestMove(charge);
            }
        } else {
            if (DEBUG_OUTPUT) {
                cerr << "Branch 3" << endl;
            }

            Tile stalk = {0, 0};
            for (auto& position : opponent_positions) {
                stalk.first  += position.first;
                stalk.second += position.second;
            }
            stalk.first  /= opponent_positions.size();
            stalk.second /= opponent_positions.size();

            if (DEBUG_OUTPUT) {
                cerr << "Possible positions: ";
                for (auto& position : opponent_positions) {
                    cerr << "{" << position.first << "," << position.second << "} ";
                }
                cerr << "\n";
                cerr << "Stalking: {" << stalk.first << ", " << stalk.second << "}" << endl;
            }

            if (distance(my_state.current_potision, stalk) > 2) {
                stalkerMove(stalk, charge);
            } else {
                longestMove(charge);
            }

            if (my_state.torpedo_cooldown == 0) {
                for (auto& position : opponent_positions) {
                    if (distance(my_state.current_potision, position) <= 4) {
                        cout << " | TORPEDO " << position.second << " " << position.first;
                        break;
                    }
                }
            }
        }

        mineActions();
    }

    void move();
    void run();

public:
    struct PlayerState {
        int life = 6;
        int torpedo_cooldown = 3;
        int sonar_cooldown = 4;
        int silence_cooldown = 6;
        int mine_cooldown = 3;

        Tile current_potision = {0, 0};

        bool hit = false;
        bool surfaced = false;
        bool must_move_silently = false;
    };

    PlayerState my_state;
    PlayerState opponent_state;

    int width  = 15;
    int height = 15;
    int my_id  = 0;

    int frame = -1;

    static constexpr int TILE_FREE   =  0;
    static constexpr int TILE_ISLAND = -1;

    static constexpr int max_dfs_depth_for_longest_path       = 15;
    static constexpr int stalk_when_positions_less_than       = 10;
    static constexpr int do_not_surface_when_stalk_below_life = 6;
    static constexpr int part_to_trigger_mine = 2;
    static constexpr int mines_before_silence = 3;
    int current_mines_before_silence = 0;

    static constexpr bool DEBUG_OUTPUT = true;

    static constexpr int directions = 4;
    vector < int > directions_numbers = {0, 1, 2, 3};
    int d_row[directions] = {-1, +1,  0,  0};
    int d_col[directions] = { 0,  0, -1, +1};
    string directions_names = "NSWE";

    vector < vector < int > > game_map;
    vector < Tile > opponent_positions;
    unordered_set < Tile , TileHash > visited_tiles;
    unordered_set < Tile , TileHash > mines;

public:
    inline void printDirectionNumbers() const;

    inline int distance(const Tile& t1, const Tile& t2) const;
    inline int distance(const Tile& t1, int row2, int col2) const;
    inline int distance(int row1, int col1, const Tile& t2) const;
    inline int distance(int row1, int col1, int row2, int col2) const;

    inline bool isInsideMap(const Tile& tile) const;
    inline bool isInsideMap(int row, int col) const;

    inline bool isIsland(const Tile& tile) const;
    inline bool isIsland(int row, int col) const;

    inline bool isVisited(const Tile& tile) const;
    inline bool isVisited(int row, int col) const;

    inline int sectorNumber(const Tile& tile) const;
    inline int sectorNumber(int row, int col) const;

    inline bool isInSector(const Tile& tile, int sector) const;
    inline bool isInSector(int row, int col, int sector) const;

    inline bool canMove(const Tile& from, const Tile& to) const;
    inline bool canMove(const Tile& from, int to_row, int to_col) const;
    inline bool canMove(int from_row, int from_col, const Tile& to) const;
    inline bool canMove(int from_row, int from_col, int to_row, int to_col) const;

    inline void decreaseCharged(const string& charge);
    inline int directionNumber(char c) const;

    inline char findDirection(const Tile& from, const Tile& to) const;
    inline char findDirection(const Tile& from, int to_row, int to_col) const;
    inline char findDirection(int from_row, int from_col, const Tile& to) const;
    inline char findDirection(int from_row, int from_col, int to_row, int to_col) const;

    inline bool isPlacedMine(const Tile& tile) const;
    inline bool isPlacedMine(int row, int column) const;

    inline bool canPlaceMine(const Tile& tile) const;
    inline bool canPlaceMine(int row, int column) const;

    string whatToCharge() const;

    inline void printMyPosition() const;
    inline void insertMyPositionInVisitedTiles();
    inline void getMyRandomPosition();
    inline void printOpponentsPositions() const;
};

int main() {
    Game game;

    game.run();

    return 0;
}

/******************
 * IMPLEMENTATION *
 ******************/

void lTrim(string& source) {
    source.erase(source.begin(),
                 find_if(source.begin(),
                         source.end(),
                         [](int ch) {return !isspace(ch);}
                         )
                );
    return;
}

void rTrim(string& source) {
    source.erase(find_if(source.rbegin(),
                         source.rend(),
                         [](int ch) {return !isspace(ch);}
                         ).base(),
                         source.end()
                );
    return;
}

void trim(string& source) {
    lTrim(source);
    rTrim(source);

    return;
}

vector < string > splitString(const string& source, char split_symbol) {
    stringstream ss(source);
    string str;
    vector < string > result;
    while (getline(ss, str, split_symbol)) {
        trim(str);
        if (!str.empty()) {
            result.push_back(std::move(str));
        }
    }
    return result;
}

void Game::printDirectionNumbers() const {
    //*
    if (DEBUG_OUTPUT) {
        cerr << "Direction numbers after shuffle: " << endl;
        for (const auto& direction : directions_numbers) {
            cerr << direction << " ";
        }
        cerr << endl;
    }
    //*/
}

int Game::distance(const Tile& t1, const Tile& t2) const {
    return distance(t1.first, t1.second, t2.first, t2.second);
}

int Game::distance(const Tile& t1, int row2, int col2) const {
    return distance(t1.first, t1.second, row2, col2);
}

int Game::distance(int row1, int col1, const Tile& t2) const {
    return distance(row1, col1, t2.first, t2.second);
}

int Game::distance(int row1, int col1, int row2, int col2) const {
    return abs(row1 - row2) + abs(col1 - col2);
}

bool Game::isInsideMap(const Tile& tile) const {
    return isInsideMap(tile.first, tile.second);
}

bool Game::isInsideMap(int row, int col) const {
    return ((0 <= row && row < width) &&
            (0 <= col && col < height));
}

bool Game::isIsland(const Tile& tile) const {
    return isIsland(tile.first, tile.second);
}

bool Game::isIsland(int row, int col) const {
    return game_map[row][col] == TILE_ISLAND;
}

bool Game::isVisited(const Tile& tile) const {
    return isVisited(tile.first, tile.second);
}

bool Game::isVisited(int row, int col) const {
    return visited_tiles.count(Tile{row, col}) > 0;
}

int Game::sectorNumber(const Tile& tile) const {
    return sectorNumber(tile.first, tile.second);
}

int Game::sectorNumber(int row, int col) const {
    int row_sector = row / 5;
    int col_sector = col / 5;

    if (row_sector == 0) {
        return 1 + col_sector;
    }
    if (row_sector == 1) {
        return 4 + col_sector;
    }
    if (row_sector == 2) {
        return 7 + col_sector;
    }

    return -1;
}

bool Game::isInSector(const Tile& tile, int sector) const {
    return isInSector(tile.first, tile.second, sector);
}

bool Game::isInSector(int row, int col, int sector) const {
    return sectorNumber(row, col) == sector;
}

bool Game::canMove(const Tile& from, const Tile& to) const {
    return canMove(from.first, from.second, to.first, to.second);
}

bool Game::canMove(const Tile& from, int to_row, int to_col) const {
    return canMove(from.first, from.second, to_row, to_col);
}

bool Game::canMove(int from_row, int from_col, const Tile& to) const {
    return canMove(from_row, from_col, to.first, to.second);
}

bool Game::canMove(int from_row, int from_col, int to_row, int to_col) const {
    if (!isInsideMap(from_row, from_col) ||
        !isInsideMap(  to_row,   to_col)) {
        return false;
    }

    if (from_row != to_row &&
        from_col != to_col) {
        return false;
    }

    if (from_col == to_col) {
        int dd_row = +1;
        if (from_row > to_row) {
            dd_row = -1;
        }
        for (int row = from_row; row != to_row; row += dd_row) {
            if (game_map[row][from_col] == TILE_ISLAND) {
                return false;
            }
        }
    } else {
        int dd_col = +1;
        if (from_col > to_col) {
            dd_col = -1;
        }
        for (int col = from_col; col != to_col; col += dd_col) {
            if (game_map[from_row][col] == TILE_ISLAND) {
                return false;
            }
        }
    }

    return true;
}

void Game::decreaseCharged(const string& charge) {
    if (charge == "TORPEDO") {
        my_state.torpedo_cooldown = max(0, my_state.torpedo_cooldown - 1);
    } else if (charge == "SILENCE") {
        my_state.silence_cooldown = max(0, my_state.silence_cooldown - 1);
    } else if (charge == "MINE") {
        my_state.mine_cooldown    = max(0, my_state.mine_cooldown    - 1);
    } else if (charge == "SONAR") {
        my_state.sonar_cooldown   = max(0, my_state.sonar_cooldown   - 1);
    }
}

int Game::directionNumber(char c) const {
    for (int direction = 0; direction < directions; ++direction) {
        if (directions_names[direction] == c) {
            return direction;
        }
    }

    if (DEBUG_OUTPUT) {
        cerr << "No such direction : " << c << endl;
    }

    return -1;
}

char Game::findDirection(const Tile& from, const Tile& to) const {
    return findDirection(from.first, from.second, to.first, to.second);
}

char Game::findDirection(const Tile& from, int to_row, int to_col) const {
    return findDirection(from.first, from.second, to_row, to_col);
}

char Game::findDirection(int from_row, int from_col, const Tile& to) const {
    return findDirection(from_row, from_col, to.first, to.second);
}

char Game::findDirection(int from_row, int from_col, int to_row, int to_col) const {
    if (from_row == to_row) {
        if (from_col < to_col) {
            return 'E';
        } else {
            return 'W';
        }
    } else {
        if (from_row < to_row) {
            return 'S';
        } else {
            return 'N';
        }
    }
}

bool Game::isPlacedMine(const Tile& tile) const {
    return isPlacedMine(tile.first, tile.second);
}

bool Game::isPlacedMine(int row, int column) const {
    return mines.count({row, column});
}

bool Game::canPlaceMine(const Tile& tile) const {
    return canPlaceMine(tile.first, tile.second);
}

bool Game::canPlaceMine(int row, int column) const {
    return (isInsideMap(row, column) &&
            !isIsland(row, column) &&
            !isPlacedMine(row, column));
}

string Game::whatToCharge() const {
    if (my_state.silence_cooldown > 0) {
        return "SILENCE";
    }
    if (my_state.torpedo_cooldown > 0) {
        return "TORPEDO";
    }
    if (my_state.mine_cooldown > 0) {
        return "MINE";
    }

    return "TORPEDO";
}

void Game::setMine(const Tile& tile) {
    return setMine(tile.first, tile.second);
}

int Game::dfs(const Tile& tile, int depth) {
    return dfs(tile.first, tile.second, depth);
}

void Game::printMyPosition() const {
    cout << my_state.current_potision.second << " " << my_state.current_potision.first;
}

void Game::insertMyPositionInVisitedTiles() {
    visited_tiles.insert(my_state.current_potision);
}

void Game::getMyRandomPosition() {
    vector < Tile > possible_positions;
    possible_positions.reserve(width * height);
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            if (game_map[row][column] == TILE_FREE) {
                possible_positions.emplace_back(Tile{row, column});
            }
        }
    }

    my_state.current_potision = possible_positions[rand() % possible_positions.size()];
}

void Game::printOpponentsPositions() const {
    cout << " | MSG " << opponent_positions.size();
}

void Game::surfaceMove() {
    if (DEBUG_OUTPUT) {
        cerr << "Surface move" << endl;
    }

    visited_tiles.clear();
    insertMyPositionInVisitedTiles();
    my_state.surfaced = true;
    cout << "SURFACE";
}

void Game::run() {
    readMapData();
    getMyRandomPosition();
    insertMyPositionInVisitedTiles();
    printMyPosition();
    cout << endl;

    while (true) {
        readState();
        move();
    }
}

void Game::move() {
    ++frame;
    my_state.surfaced = false;

    shootMove(whatToCharge());

    printOpponentsPositions();

    cout << endl;
}
