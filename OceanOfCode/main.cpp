#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <set>
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

        if (order[0] == "move") {
            cout << " move ";
            int direction = -1;

            if (order[1] == "n") {
                cout << "n";
                direction = 0;
            } else if (order[1] == "s") {
                cout << "s";
                direction = 1;
            } else if (order[1] == "w") {
                cout << "w";
                direction = 2;
            } else if (order[1] == "e") {
                cout << "e";
                direction = 3;
            } else {
                cout << "unknow direction: " << order[1];
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

            cout << " pos: " << opponent_positions.size();
        } else if (order[0] == "torpedo") {
            cout << " torpedo";
        } else if (order[0] == "surface") {
            cout << " surface";
        } else if (order[0] == "na") {
            cout << " no action";
        } else {
            cout << " unknown";
        }
    }

    void parseOpponentOrders(string& opponent_orders) {
        transform(opponent_orders.begin(), opponent_orders.end(), opponent_orders.begin(), ::tolower);

        for (auto& symbol : opponent_orders) {
            if (symbol == '|') {
                symbol = '&';
            }
        }

        cout << "MSG";
        vector < string > orders = splitString(opponent_orders, '&');
        for (auto& order : orders) {
            updatePossibleOpponentsPositions(order);
        }
        cout << " | ";
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

    void printMyPosition(bool flush = true) {
        cout << my_state.current_potision.second << " " << my_state.current_potision.first;
        if (flush) {
            cout << endl;
        }
    }

    void insertMyPositionInVisitedTiles() {
        visited_tiles.insert(my_state.current_potision);
    }

    bool isInsideMap(const Tile& tile) {
        return isInsideMap(tile.first, tile.second);
    }

    bool isInsideMap(int row, int col) {
        return ((0 <= row && row < width) &&
                (0 <= col && col < height));
    }

    bool isIsland(const Tile& tile) {
        return isIsland(tile.first, tile.second);
    }

    bool isIsland(int row, int col) {
        return game_map[row][col] == TILE_ISLAND;
    }

    bool isVisited(int row, int col) {
        return visited_tiles.count(Tile{row, col}) > 0;
    }

    void randomMove() {
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
            cout << "MOVE " << directions_names[found_direction] << " TORPEDO" << endl;
        } else {
            visited_tiles.clear();
            insertMyPositionInVisitedTiles();
            cout << "SURFACE" << endl;
        }
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

    const int TILE_FREE   =  0;
    const int TILE_ISLAND = -1;

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
        game.randomMove();
    }
}
