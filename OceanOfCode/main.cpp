#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <assert.h>

using namespace std;

typedef pair < int, int > Tile;

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

    bool isInsideMap(int row, int col) {
        return ((0 <= row && row < width) &&
                (0 <= col && col < height));
    }

    bool isIsland(int row, int col) {
        return game_map[row][col] == TILE_ISLAND;
    }

    bool isVisited(int row, int col) {
        return visited_tiles.count(Tile{row, col}) > 0;
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

    // game loop
    while (true) {
        /*
        int x;
        int y;
        int myLife;
        int oppLife;
        int torpedoCooldown;
        int sonarCooldown;
        int silenceCooldown;
        int mineCooldown;
        cin >> x >> y >> myLife >> oppLife >> torpedoCooldown >> sonarCooldown >> silenceCooldown >> mineCooldown; cin.ignore();
        string sonarResult;
        cin >> sonarResult; cin.ignore();
        string opponentOrders;
        getline(cin, opponentOrders);
        //*/
//        cin >> game.my_state.current_potision.second >> game.my_state.current_potision.first;

        int found_direction = -1;
        vector < int > directions = {0, 1, 2, 3};
        random_shuffle(directions.begin(), directions.end());
        for (const auto& current_direction : directions) {
            int new_row = game.my_state.current_potision.first  + game.d_row[current_direction];
            int new_col = game.my_state.current_potision.second + game.d_col[current_direction];
            if (game.isInsideMap(new_row, new_col) &&
                !game.isIsland(new_row, new_col) &&
                !game.isVisited(new_row, new_col)) {
                found_direction = current_direction;
                break;
            }
        }

        if (found_direction >= 0) {
            game.my_state.current_potision.first  += game.d_row[found_direction];
            game.my_state.current_potision.second += game.d_col[found_direction];
            game.insertMyPositionInVisitedTiles();
            cout << "MOVE " << game.directions_names[found_direction] << " TORPEDO" << endl;
        } else {
            game.visited_tiles.clear();
            game.insertMyPositionInVisitedTiles();
            cout << "SURFACE" << endl;
        }
    }
}
