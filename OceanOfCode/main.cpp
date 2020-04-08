// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <queue>
#include <assert.h>

using namespace std;

static auto _ = [] () { ios_base::sync_with_stdio(false); cin.tie(nullptr); return 0; } ();

typedef pair < int, int > Tile;

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

vector < string > splitString(const string& source, const char& split_symbol) {
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

class Game {
public:

    void readMapData() {
        cin >> width >> height >> my_id;
        game_map.resize(height);
        assert(height == 15);
        for (int row = 0; row < height; row++) {
            string line;
            cin >> line;
            assert(static_cast < int >(line.length()) == width);
            game_map[row].resize(width);
            for (int column = 0; column < width; ++column) {
                if (line[column] == '.') {
                    game_map[row][column] = TILE_FREE;
                    opponent_positions.push_back({row, column});
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

    void updatePossibleOpponentsPositions(string& opponent_order) {
        if (DEBUG_OUTPUT) {
            cerr << "Parsing order: " << opponent_order << endl;
        }

        vector < string > order = splitString(opponent_order, ' ');
        stringstream out;

        if (order[0] == "move") {
            out << "Move ";
            int direction = -1;

            if (order[1] == "n") {
                out << "N";
                direction = 0;
            } else if (order[1] == "s") {
                out << "S";
                direction = 1;
            } else if (order[1] == "w") {
                out << "W";
                direction = 2;
            } else if (order[1] == "e") {
                out << "E";
                direction = 3;
            } else {
                out << "unknown move: " << order[1];
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

            out << "\nPositions: " << opponent_positions.size() << "\n";
        } else if (order[0] == "torpedo") {
            out << "Torpedo";

            int x = atoi(order[1].c_str());
            int y = atoi(order[2].c_str());

            out << " at {" << x << "," << y << "}";

            auto it = partition(opponent_positions.begin(),
                                opponent_positions.end(),
                                [this, x, y](Tile& tile) {
                                    return distance(tile, {y, x}) <= 4;
                                });
            opponent_positions.resize(it - opponent_positions.begin());

            out << "\nPositions: " << opponent_positions.size() << "\n";
        } else if (order[0] == "surface") {
            out << "Surface";

            int sector = atoi(order[1].c_str());
            out << " at sector {" << sector << "}";

            auto it = partition(opponent_positions.begin(),
                                opponent_positions.end(),
                                [this, sector](Tile& tile) {
                                    return isInSector(tile, sector);
                                });
            opponent_positions.resize(it - opponent_positions.begin());

            out << "\nPositions: " << opponent_positions.size() << "\n";
        } else if (order[0] == "sonar") {
            int sector = atoi(order[1].c_str());

            out << " Sonar scanned region " << sector << "\n";
        } else if (order[0] == "silence") {
            out << "Silence";

            set < Tile > new_opponent_positions;

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

            out << "\nPositions: " << opponent_positions.size() << "\n";
        } else if (order[0] == "na") {
            out << "No action\n";
        } else {
            out << "Unknown action: " << order[0] << "\n";
        }

        if (DEBUG_OUTPUT) {
            cerr << out.str() << "\n";
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

        for (auto& order : orders) {
            updatePossibleOpponentsPositions(order);
        }

        if (DEBUG_OUTPUT) {
            if (opponent_positions.size() == 1) {
                cerr << "Opponent position at {" << opponent_positions[0].first << "," << opponent_positions[0].second << "}\n";
            }
        }
    }

    void getMyRandomPosition() {
        vector < Tile > possible_positions;
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                if (game_map[row][column] == TILE_FREE) {
                    possible_positions.emplace_back(Tile{row, column});
                }
            }
        }

        my_state.current_potision = possible_positions[rand() % possible_positions.size()];
    }

    void printMyPosition(bool flush) const {
        cout << my_state.current_potision.second << " " << my_state.current_potision.first;
        if (flush) {
            cout << endl;
        }
    }

    void insertMyPositionInVisitedTiles() {
        visited_tiles.insert(my_state.current_potision);
    }

    void makeMove(int direction, const string& charge, bool flush) {
        insertMyPositionInVisitedTiles();
        cout << "MOVE " << directions_names[direction] << " " << charge;
        if (flush) {
            cout << endl;
        }
    }

    int distance(const Tile& t1, const Tile& t2) const {
        return abs(t1.first - t2.first) + abs(t1.second - t2.second);
    }

    bool isInsideMap(const Tile& tile) const {
        return isInsideMap(tile.first, tile.second);
    }

    bool isInsideMap(int row, int col) const {
        return ((0 <= row && row < width) &&
                (0 <= col && col < height));
    }

    bool isIsland(const Tile& tile) const {
        return isIsland(tile.first, tile.second);
    }

    bool isIsland(int row, int col) const {
        return game_map[row][col] == TILE_ISLAND;
    }

    bool isVisited(const Tile& tile) const {
        return isVisited(tile.first, tile.second);
    }

    bool isVisited(int row, int col) const {
        return visited_tiles.count(Tile{row, col}) > 0;
    }

    int sectorNumber(const Tile& tile) const {
        return sectorNumber(tile.first, tile.second);
    }

    int sectorNumber(int row, int col) const {
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

    bool isInSector(const Tile& tile, int sector) const {
        return isInSector(tile.first, tile.second, sector);
    }

    bool isInSector(int row, int col, int sector) const {
        return sectorNumber(row, col) == sector;
    }

    bool canMove(const Tile& from, const Tile& to) const {
        return canMove(from.first, from.second, to.first, to.second);
    }

    bool canMove(const Tile& from, int to_row, int to_col) const {
        return canMove(from.first, from.second, to_row, to_col);
    }

    bool canMove(int from_row, int from_col, const Tile& to) const {
        return canMove(from_row, from_col, to.first, to.second);
    }

    bool canMove(int from_row, int from_col, int to_row, int to_col) const {
        if (from_row != to_row && from_col != to_col) {
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

            return true;
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

            return true;
        }
    }

    void surfaceMove(bool flush) {
        if (DEBUG_OUTPUT) {
            cerr << "Surface move" << endl;
        }

        visited_tiles.clear();
        insertMyPositionInVisitedTiles();
        my_state.surfaced = true;
        cout << "SURFACE";
        if (flush) {
            cout << endl;
        }
    }

    void randomMove(const string& charge, bool flush) {
        if (DEBUG_OUTPUT) {
            cerr << "Random move" << endl;
        }

        int found_direction = -1;
        vector < int > directions = {0, 1, 2, 3};
        random_shuffle(directions.begin(), directions.end());
        for (const auto& current_direction : directions) {
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
            makeMove(found_direction, charge, flush);
        } else {
            surfaceMove(flush);
        }
    }

    int dfs(const Tile& tile, int depth = 0) {
        return dfs(tile.first, tile.second, depth);
    }

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
        vector < int > directions = {0, 1, 2, 3};
        random_shuffle(directions.begin(), directions.end());
        for (const auto& current_direction : directions) {
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

    void longestMove(const string& charge, bool flush) {
        if (DEBUG_OUTPUT) {
            cerr << "Longest move" << endl;
        }

        int max_length = 0;
        int max_length_direction = -1;
        vector < int > directions = {0, 1, 2, 3};
        random_shuffle(directions.begin(), directions.end());
        for (const auto& current_direction : directions) {
            int new_row = my_state.current_potision.first  + d_row[current_direction];
            int new_col = my_state.current_potision.second + d_col[current_direction];
            int current_length = dfs(new_row, new_col);

            if (DEBUG_OUTPUT) {
//                cerr << "Direction : " << directions_names[current_direction] << ", length = " << current_length << endl;
            }

            if (current_length > max_length) {
                max_length = current_length;
                max_length_direction = current_direction;
            }
        }

        if (max_length_direction >= 0) {
            my_state.current_potision.first  += d_row[max_length_direction];
            my_state.current_potision.second += d_col[max_length_direction];
            makeMove(max_length_direction, charge, flush);
        } else {
            surfaceMove(flush);
        }
    }

    int directionNumber(char c) {
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

    char findDirection(const Tile& from, const Tile& to) {
        return findDirection(from.first, from.second, to.first, to.second);
    }

    char findDirection(const Tile& from, int to_row, int to_col) {
        return findDirection(from.first, from.second, to_row, to_col);
    }

    char findDirection(int from_row, int from_col, const Tile& to) {
        return findDirection(from_row, from_col, to.first, to.second);
    }

    char findDirection(int from_row, int from_col, int to_row, int to_col) {
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

    void stalkerMove(const Tile& stalk, const string& charge, bool flush) {
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
                surfaceMove(flush);
            } else {
                longestMove(charge, flush);
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
            makeMove(direction, charge, flush);
        }
    }

    void silentMove(bool flush) {
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
            surfaceMove(flush);

            my_state.must_move_silently = true;
        } else {
            for (int step = 1; step <= found_length; ++step) {
                visited_tiles.insert({my_state.current_potision.first  + step * d_row[found_direction],
                                      my_state.current_potision.second + step * d_col[found_direction]});
            }
            my_state.current_potision.first  += found_length * d_row[found_direction];
            my_state.current_potision.second += found_length * d_col[found_direction];
            insertMyPositionInVisitedTiles();

            cout << "SILENCE " << directions_names[found_direction] << " " << found_length;

            if (flush) {
                cout << endl;
            }
        }
    }

    void mineActions(bool flush) {
        /// TODO

        if (flush) {
            cout << endl;
        }
    }

    bool setMine(bool flush) {
        /// TODO

        return true;
    }

    bool triggerMine(bool flush) {
        /// TODO

        return false;
    }

    void shootMove(const string& charge, bool flush) {
        if (DEBUG_OUTPUT) {
            cerr << "Shoot move" << endl;
        }

        if ((my_state.silence_cooldown == 0) && (my_state.hit || my_state.must_move_silently)) {
            silentMove(false);
            mineActions(true);
        } else if (opponent_positions.size() > stalk_when_positions_less_than) {
            longestMove(charge, false);
            mineActions(true);
        } else {
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
                stalkerMove(stalk, charge, false);
            } else {
                longestMove(charge, false);
            }

            mineActions(false);

            if (!my_state.torpedo_cooldown) {
                for (auto& position : opponent_positions) {
                    if (distance(my_state.current_potision, position) <= 4) {
                        cout << " | TORPEDO " << position.second << " " << position.first;
                        break;
                    }
                }
            }
            cout << endl;
        }
    }

    string whatToCharge() const {
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

    void move() {
        my_state.surfaced = false;

        shootMove(whatToCharge(), true);
    }

    void run() {
        readMapData();
        getMyRandomPosition();
        printMyPosition(true);
        insertMyPositionInVisitedTiles();

        while (true) {
            readState();
            move();
        }
    }

public:
    struct PlayerState {
        int life = 6;
        int torpedo_cooldown;
        int sonar_cooldown;
        int silence_cooldown;
        int mine_cooldown;

        Tile current_potision;

        bool hit = false;
        bool surfaced = false;
        bool must_move_silently = false;
    };

    PlayerState my_state;
    PlayerState opponent_state;

    int width;
    int height;
    int my_id;

    int frame = -1;

    static constexpr int TILE_FREE   =  0;
    static constexpr int TILE_ISLAND = -1;

    static constexpr int max_dfs_depth_for_longest_path       = 20;
    static constexpr int stalk_when_positions_less_than       = 5;
    static constexpr int do_not_surface_when_stalk_below_life = 3;

    static constexpr bool DEBUG_OUTPUT = true;

    static constexpr int directions = 4;
    int d_row[directions] = {-1, +1,  0,  0};
    int d_col[directions] = { 0,  0, -1, +1};
    string directions_names = "NSWE";

    vector < vector < int > > game_map;
    vector < Tile > opponent_positions;
    set < Tile > visited_tiles;
    set < Tile > mines;
};

int main() {
    Game game;
    game.run();
}
