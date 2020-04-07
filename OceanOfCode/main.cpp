#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <assert.h>

using namespace std;

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
        cin >> my_state.life;
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

        parseOpponentOrders(opponent_orders);
    }

    void updatePossibleOpponentsPositions(string& opponent_order) {
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
        } else if (order[0] == "na") {
            out << " no action\n";
        } else {
            out << " unknown action: " << order[0] << "\n";
        }

        if (DEBUG_OUTPUT) {
            cerr << out.str() << "\n";
        }
    }

    void parseOpponentOrders(string& opponent_orders) {
        transform(opponent_orders.begin(), opponent_orders.end(), opponent_orders.begin(), ::tolower);

        for (auto& symbol : opponent_orders) {
            if (symbol == '|') {
                symbol = '&';
            }
        }

        vector < string > orders = splitString(opponent_orders, '&');
        for (auto& order : orders) {
            updatePossibleOpponentsPositions(order);
        }

        if (DEBUG_OUTPUT) {
            if (opponent_positions.size() == 1) {
                cerr << " opponent position at {" << opponent_positions[0].first << "," << opponent_positions[0].second << "}\n";
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

    void printMyPosition(bool flush = true) const {
        cout << my_state.current_potision.second << " " << my_state.current_potision.first;
        if (flush) {
            cout << endl;
        }
    }

    void insertMyPositionInVisitedTiles() {
        visited_tiles.insert(my_state.current_potision);
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

    void randomMove(const string& charge = "TORPEDO", bool flush = true) {
        int found_direction = -1;
        vector < int > directions = {0, 1, 2, 3};
        random_shuffle(directions.begin(), directions.end());
        for (const auto& current_direction : directions) {
            int new_row = my_state.current_potision.first  + d_row[current_direction];
            int new_col = my_state.current_potision.second + d_col[current_direction];
            if (isInsideMap(new_row, new_col) &&
                !isIsland(new_row, new_col) &&
                !isVisited(new_row, new_col)) {
                found_direction = current_direction;
                break;
            }
        }

        if (found_direction >= 0) {
            my_state.current_potision.first  += d_row[found_direction];
            my_state.current_potision.second += d_col[found_direction];
            insertMyPositionInVisitedTiles();
            cout << "MOVE " << directions_names[found_direction] << " " << charge;
            if (flush) {
                cout << endl;
            }
        } else {
            visited_tiles.clear();
            insertMyPositionInVisitedTiles();
            cout << "SURFACE";
            if (flush) {
                cout << endl;
            }
        }
    }

    int dfs(const Tile& tile, int depth = 0) {
        return dfs(tile.first, tile.second, depth);
    }

    int dfs(int row, int col, int depth = 0) {
//        cerr << "DFS from {" << row << "," << col << "}" << endl;

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

    void longestMove(const string& charge = "TORPEDO", bool flush = true) {
        int max_length = 0;
        int max_length_direction = -1;
        vector < int > directions = {0, 1, 2, 3};
        random_shuffle(directions.begin(), directions.end());
        for (const auto& current_direction : directions) {
            int new_row = my_state.current_potision.first  + d_row[current_direction];
            int new_col = my_state.current_potision.second + d_col[current_direction];
            int current_length = dfs(new_row, new_col);

            if (DEBUG_OUTPUT) {
                cerr << "Direction : " << directions_names[current_direction] << ", length = " << current_length << endl;
            }

            if (current_length > max_length) {
                max_length = current_length;
                max_length_direction = current_direction;
            }
        }

        if (max_length_direction >= 0) {
            my_state.current_potision.first  += d_row[max_length_direction];
            my_state.current_potision.second += d_col[max_length_direction];
            insertMyPositionInVisitedTiles();
            cout << "MOVE " << directions_names[max_length_direction] << " " << charge;
            if (flush) {
                cout << endl;
            }
        } else {
            visited_tiles.clear();
            insertMyPositionInVisitedTiles();
            cout << "SURFACE";
            if (flush) {
                cout << endl;
            }
        }
    }

    void stalkerMove(const string& charge = "TORPEDO", bool flush = true) {
        longestMove(charge, flush);
    }

    void shootMove(const string& charge = "TORPEDO", bool flush = true) {
        if (opponent_positions.size() > 1) {
            longestMove(charge);
        } else {
            stalkerMove(charge, false);
            if (!my_state.torpedo_cooldown &&
                distance(my_state.current_potision, opponent_positions[0]) <= 4) {
                    cout << " | TORPEDO " << opponent_positions[0].second << " " << opponent_positions[0].first;
                }
            cout << endl;
        }
    }

    string whatToCharge() const {
        if (my_state.torpedo_cooldown > 0) {
            return "TORPEDO";
        }

        return "TORPEDO";
    }

    void move() {
        shootMove(whatToCharge());
    }

public:
    struct PlayerState {
        int life;
        int torpedo_cooldown;
        int sonar_cooldown;
        int silence_cooldown;
        int mine_cooldown;

        Tile current_potision;
    };

    PlayerState my_state;
    PlayerState opponent_state;

    int width;
    int height;
    int my_id;

    int frame = -1;

    static constexpr int TILE_FREE   =  0;
    static constexpr int TILE_ISLAND = -1;

    static constexpr int max_dfs_depth_for_longest_path = 20;

    static constexpr bool DEBUG_OUTPUT = false;

    static constexpr int directions = 4;
    int d_row[directions] = {-1, +1,  0,  0};
    int d_col[directions] = { 0,  0, -1, +1};
    string directions_names = "NSWE";

    vector < vector < int > > game_map;
    vector < Tile > opponent_positions;
    set < Tile > visited_tiles;
};

int main() {
    Game game;

    game.readMapData();
    game.getMyRandomPosition();
    game.printMyPosition();
    game.insertMyPositionInVisitedTiles();

    while (true) {
        game.readState();
        game.move();
    }
}
