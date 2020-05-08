// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <math.h>

//#define CTIME

#ifdef CTIME
#include <time.h>
#else
#include <sys/time.h>
#endif // CTIME

using namespace std;

constexpr float PI = 3.1415926;
constexpr float EPS = 1E-3;

constexpr int MAX_ZOMBIES = 100;
constexpr int MAX_HUMANS  = 100;

constexpr float MAX_EVALUATION_TIME = 135;
constexpr int MAX_MOVES   = 100;

constexpr int PLAYER_ID = -1;
constexpr int TILE_ID = -2;
constexpr int EMPTY_ZOMBIE = -4;
constexpr int EMPTY_HUMAN = -5;

constexpr float PLAYER_SHOOT_RANGE = 2000;
constexpr float PLAYER_MOVE_RANGE  = 1000;
constexpr float ZOMBIE_MOVE_RANGE  = 400;

/// Supplementary

template < typename T >
inline T abs(T x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

template < typename T >
inline T sqr(T x) {
    return x * x;
}

template < typename T >
inline bool equal(T a, T b) noexcept {
    return abs(a - b) < EPS;
}

inline int randomInt(int max_value) noexcept {
    return rand() % max_value;
}

inline int toInt(float x) noexcept {
    return static_cast < int > (floor(x));
}

template < typename T >
inline float randomFloat(T max_value) noexcept {
    return static_cast < float > (max_value) * rand() / RAND_MAX;
}

constexpr int max_fibonacci = 40;
int fibonacci[max_fibonacci];

void fillFibonacci() noexcept {
    fibonacci[0] = fibonacci[1] = 1;
    for (int i = 2; i < max_fibonacci; ++i) {
        fibonacci[i] = fibonacci[i-1] + fibonacci[i-2];
    }
}

#ifdef CTIME

inline auto tick() noexcept {
    return clock();
}

inline float timing(const clock_t& t0, const clock_t& t1) noexcept {
	return static_cast < float > (1000) * (t1 - t0) / CLOCKS_PER_SEC;
}

#else

inline auto tick() noexcept {
    timeval t;
    gettimeofday(&t, nullptr);
    return t;
}

inline float timing(const timeval& t0, const timeval& t1) noexcept {
	return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

#endif // CTIME

/// Game objects

class Point {
public:
    Point() noexcept
        : x(0)
        , y(0) {
    }

    Point(float _x) noexcept
        : x(_x)
        , y(_x) {
    }

    Point(float _x, float _y) noexcept
        : x(_x)
        , y(_y) {
    }

    bool operator == (const Point& other) const noexcept {
        return equal(x, other.x) && equal(y, other.y);
    }

    bool operator != (const Point& other) const noexcept {
        return !(*this == other);
    }

    bool operator < (const Point& other) const noexcept {
        return length() < other.length();
    }

    bool operator > (const Point& other) const noexcept {
        return other < *this;
    }

    bool operator <= (const Point& other) const noexcept {
        return !(*this > other);
    }

    bool operator >= (const Point& other) const noexcept {
        return !(*this < other);
    }

    Point& operator += (const Point& other) noexcept {
        x += other.x;
        y += other.y;

        return *this;
    }

    Point& operator -= (const Point& other) noexcept {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    Point& operator *= (const Point& other) noexcept {
        x *= other.x;
        y *= other.y;

        return *this;
    }

    Point& operator /= (const Point& other)  {
        x /= other.x;
        y /= other.y;

        return *this;
    }

    Point operator + (const Point& other) const noexcept {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator - (const Point& other) const noexcept {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator * (const Point& other) const noexcept {
        Point result = *this;
        result *= other;
        return result;
    }

    Point operator / (const Point& other) const {
        Point result = *this;
        result /= other;
        return result;
    }

    float length() const noexcept {
        return sqrt(sqr(x) + sqr(y));
    }

public:
    float x = 0;
    float y = 0;
};

Point abs(const Point& p) noexcept {
    return Point(abs(p.x), abs(p.y));
}

Point floor(const Point& p) noexcept {
    return Point(floor(p.x), floor(p.y));
}

inline auto distance(const Point& p1, const Point& p2) noexcept {
    return sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
}

Point area(16000, 900);

class GameObject {
public:
    GameObject() noexcept
        : id(0)
        , position()
        , starting_position() {

    }

    GameObject(int _id) noexcept
        : id(_id)
        , position()
        , starting_position() {

    }

    GameObject(const Point &p) noexcept
        : id(0)
        , position(p)
        , starting_position(p) {

    }

    GameObject(int _id, const Point &p) noexcept
        : id(_id)
        , position(p)
        , starting_position(p){

    }

public:
    int id = 0;
    Point position;
    Point starting_position;
};

GameObject* randomTileInArea() {
    return new GameObject(Point(randomInt(area.x), randomInt(area.y)));
}

GameObject* randomTileInCircle(Point center, float radius) {
	auto angle = randomFloat(2*PI);
    return new GameObject(abs(floor(center + Point(cos(angle), sin(angle)) * Point(radius))));
}

class Human : public GameObject {

};

class Zombie {
public:
    Zombie() noexcept
        : object()
        , arrived(false)
        , target(nullptr) {

    }

public:
	GameObject object;
	bool arrived = false;
	GameObject* target = nullptr;
};

class Player {
public:
    Player() noexcept
        : object()
        , arrived(false)
        , targeting_zombie(false)
        , target_zombie(nullptr)
        , target_tile(nullptr) {

    }

public:
	GameObject object;
	bool arrived         = false;
	bool targeting_zombie = false;
	Zombie*     target_zombie;
	GameObject* target_tile;
};

class Result {
public:
    Result() noexcept
    : points(0)
    , len(0) {

    }

public:
    int points = 0;
    int len = 0;
    Point moves[MAX_MOVES];
};

int  humans_count = 0;
int zombies_count = 0;

Human  humans [MAX_HUMANS ];
Zombie zombies[MAX_ZOMBIES];

/// Simulation

class Simulation {
public:
    void setup(Player& player) noexcept {
        simulation_player = player;

        for (int i = 0; i < zombies_count; ++i) {
            simulation_zombies[i] = zombies[i];
        }

        for (int i = 0; i < humans_count; ++i) {
            simulation_humans[i] = humans[i];
        }

        failure                      = false;
        zombies_all_dead             = false;
        player_target_died_this_turn = false;

        current_points = 0;
        moves_count    = 0;
        simulation_zombies_count = zombies_count;
        simulation_humans_count  = humans_count;

        simulation_starting_random_moves_number     = 0;
        simulation_starting_random_moves_number_max = 3;
    }

    auto maxScore() const noexcept {
        int result = 0;
        int human_points = 10 * sqr(simulation_humans_count);
        for (int i = 0; i < simulation_zombies_count; ++i) {
            auto points = human_points;
            if (simulation_zombies_count > 1) {
                points *= fibonacci[i+1];
            }
            result += points;
        }

        return result;
    }

    void findPlayerTarget() {
        Zombie* zombies_do_not_target_player[MAX_ZOMBIES];

        if (simulation_starting_random_moves_number > 0) {
            simulation_player.target_tile = randomTileInArea();
            simulation_player.targeting_zombie = false;
            --simulation_starting_random_moves_number;
        } else {
            int len = 0;
            for (int i = 0; i < simulation_zombies_count; ++i) {
                if (simulation_zombies[i].target != nullptr && simulation_zombies[i].target->id != PLAYER_ID) {
                    zombies_do_not_target_player[len++] = &simulation_zombies[i];
                }
            }

            simulation_player.target_zombie = (len > 0) ? zombies_do_not_target_player[randomInt(len)] : &simulation_zombies[randomInt(simulation_zombies_count)];
            simulation_player.arrived = false;
            simulation_player.targeting_zombie = true;
        }
    }

    void findZombieTarget(Zombie* zombie) noexcept {
        zombie->arrived = false;

        auto min_distance = sqr(area.length());
        auto tmp_distance = distance(zombie->object.position, simulation_player.object.position);
        if (tmp_distance < min_distance) {
            zombie->target = &(simulation_player.object);
            min_distance = tmp_distance;
        }

        for (int i = 0; i < simulation_humans_count; ++i) {
            tmp_distance = distance(zombie->object.position, simulation_humans[i].position);
            if (tmp_distance < min_distance) {
                zombie->target = &simulation_humans[i];
                min_distance = tmp_distance;
            }
        }
    }

    auto nextPositionZombie(const Zombie& zombie, Point* destination) const noexcept {
        auto arrived = false;

        if (zombie.target != nullptr) {
            auto distance_from_target = distance(zombie.object.position, zombie.target->position);

            if (floor(distance_from_target) <= ZOMBIE_MOVE_RANGE) {
                arrived = true;
                *destination = zombie.target->position;
            } else {
                auto t = ZOMBIE_MOVE_RANGE / distance_from_target;
                *destination = zombie.object.position + floor(Point(t) * (zombie.target->position - zombie.object.position));
            }
        } else {
            *destination = Point(-1);
        }

        return arrived;
    }

    void moveZombie(Zombie* zombie) const noexcept {
        zombie->arrived = nextPositionZombie(*zombie, &zombie->object.position);
    }

    auto zombieArrivedAtTarget(const Zombie& zombie) const noexcept {
        return floor(zombie.object.position) == floor(zombie.target->position);
    }

    void zombiesEat() noexcept {
        int zombieTargetId[MAX_ZOMBIES];

        for (int i = 0; i < simulation_zombies_count; ++i) {
            zombieTargetId[i] = simulation_zombies[i].target->id;
        }

        for (int i = 0; i < simulation_zombies_count; ++i) {
            auto foundHuman = false;
            if (zombieArrivedAtTarget(simulation_zombies[i])) {
                for (int j = 0; j < simulation_humans_count && !foundHuman; ++j) {
                    if (simulation_humans[j].id == zombieTargetId[i]) {
                        for (int k = j; k < MAX_HUMANS-1; ++k) {
                            simulation_humans[k] = simulation_humans[k+1];
                        }
                        foundHuman = true;
                        --simulation_humans_count;
                    }
                }
            }
        }

        for (int i = 0; i < simulation_zombies_count; ++i) {
            for (int j = 0; j < simulation_humans_count; ++j) {
                if (simulation_humans[j].id == zombieTargetId[i]) {
                    simulation_zombies[i].target = &simulation_humans[j];
                }
            }
        }
    }

    auto zombiesInRangeOfPlayer(Zombie zombies_in_range[MAX_ZOMBIES]) const noexcept {
        int len = 0;

        for (int i = 0; i < simulation_zombies_count; i++) {
            if (floor(simulation_zombies[i].object.position - simulation_player.object.position).length() <= PLAYER_SHOOT_RANGE) {
                zombies_in_range[len++] = simulation_zombies[i];
            }
        }

        return len;
    }

    auto getPlayerDestination() const noexcept {
        Zombie* target = nullptr;
        if (simulation_player.targeting_zombie) {
            target = simulation_player.target_zombie;
            Point dest;
            nextPositionZombie(*target, &dest);
            return dest;
        } else {
            return simulation_player.target_tile->position;
        }
    }

    auto nextPositionPlayer(const Player& player, Point* destination) const noexcept {
        auto arrived = false;
        if (player.target_zombie != nullptr || player.target_tile != nullptr) {
            auto dest = getPlayerDestination();
            auto distance_from_target = distance(player.object.position, dest);

            if (floor(distance_from_target) <= PLAYER_MOVE_RANGE) {
                arrived = true;
                *destination = dest;
            } else {
                auto t = PLAYER_MOVE_RANGE / distance_from_target;
                *destination = player.object.position + floor(Point(t) * (dest - player.object.position));
            }
        } else {
            *destination = Point(-1);
        }

        return arrived;
    }

    void movePlayer(Player* player) const noexcept {
        player->arrived = nextPositionPlayer(*player, &player->object.position);
    }

    void evaluate() noexcept {
        int humanPoints = 10 * sqr(simulation_humans_count);

        Zombie killableZombies[MAX_ZOMBIES];
        auto killableZombiesLen = zombiesInRangeOfPlayer(killableZombies);

        int player_target_id = (simulation_player.targeting_zombie) ? simulation_player.target_zombie->object.id : EMPTY_ZOMBIE;

        for (int i = 0; i < killableZombiesLen; ++i) {
            auto points = humanPoints;

            if (killableZombiesLen > 1) {
                points *= fibonacci[i+1];
            }
            current_points += points;

            player_target_died_this_turn |= killableZombies[i].object.id == player_target_id ? true : false;

            auto found = false;
            for (int j = simulation_zombies_count-1; j >= 0 && !found; --j) {
                if (simulation_zombies[j].object.id == killableZombies[i].object.id) {
                    --simulation_zombies_count;
                    found = true;

                    for (int k = j; k < MAX_ZOMBIES-1; ++k) {
                        simulation_zombies[k] = simulation_zombies[k+1];
                    }
                }
            }
        }

        if (killableZombiesLen > 0) {
            for (int i = 0; i < simulation_zombies_count; ++i) {
                if (simulation_zombies[i].object.id == player_target_id) {
                    simulation_player.target_zombie = &simulation_zombies[i];
                }
            }
        }
    }

    void turn(Point move_history[MAX_MOVES]) {
        for (int i = 0; i < simulation_zombies_count; i++) {
            findZombieTarget(&simulation_zombies[i]);
            moveZombie(&simulation_zombies[i]);
        }

        move_history[moves_count++] = getPlayerDestination();
        movePlayer(&simulation_player);

        evaluate();
        zombiesEat();

        if (simulation_humans_count > 0 && simulation_zombies_count > 0) {
            if (simulation_player.arrived || player_target_died_this_turn) {
                findPlayerTarget();
                player_target_died_this_turn = false;
            }
        } else {
            failure = simulation_humans_count <= 0;
            zombies_all_dead = simulation_zombies_count <= 0;
        }
    }

    auto simulate() {
        Result simulation_result;

        for (int i = 0; i < MAX_MOVES; i++) {
            simulation_result.moves[i] = Point(-1);
        }
        simulation_result.points = 0;

        simulation_starting_random_moves_number = randomInt(simulation_starting_random_moves_number_max + 1);

        findPlayerTarget();

        while (!zombies_all_dead && !failure && moves_count < MAX_MOVES) {
            if ((maxScore() + current_points) < current_best) {
                failure = true;
            }
            turn(simulation_result.moves);
        }

        if (zombies_all_dead && !failure) {
            simulation_result.points = current_points;
            simulation_result.len = moves_count;
        }

        return simulation_result;
    }

public:
    Player simulation_player = Player();
    Human  simulation_humans [MAX_HUMANS+1];
    Zombie simulation_zombies[MAX_ZOMBIES];

    bool failure                      = false;
    bool zombies_all_dead             = false;
    bool player_target_died_this_turn = false;

    int current_best   = 0;
    int current_points = 0;
    int moves_count    = 0;
    int simulation_zombies_count = 0;
    int simulation_humans_count  = 0;

    int simulation_starting_random_moves_number     = -1;
    int simulation_starting_random_moves_number_max =  3;

    int run = 0;
};

inline void init() noexcept {
	srand(time(nullptr));
    fillFibonacci();
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
}

int main() {
    Player player;
    Simulation simulation;
    Result best_result;

    init();

	for (int i = 0; i < MAX_MOVES; ++i) {
		best_result.moves[i].x = best_result.moves[i].y = -1;
	}
	best_result.points = 0;

    int move_number = 0;
	while (true) {
		cin >> player.object.position.x >> player.object.position.y;
		player.object.id = PLAYER_ID;
		player.object.starting_position = player.object.position;
		player.arrived = false;
		player.target_zombie = nullptr;
		player.targeting_zombie = false;
		player.target_tile = nullptr;

		cin >> humans_count;
        for (int i = 0; i < humans_count; ++i) {
            auto& human = humans[i];
            cin >> human.id >> human.position.x >> human.position.y;
            human.starting_position = human.position;
		}

		cin >> zombies_count;
        for (int i = 0; i < zombies_count; ++i) {
            auto& zombie = zombies[i];
            cin >> zombie.object.id >> zombie.object.position.x >> zombie.object.position.y;
            int t;
            cin >> t >> t;
            zombie.object.starting_position = zombie.object.position;
			zombie.arrived = false;
			zombie.target = nullptr;
		}

        auto new_best = false;
        simulation.run = 0;
        float total_timing = 0;

		while (total_timing < MAX_EVALUATION_TIME) {
            auto t0 = tick();

			simulation.setup(player);
			auto current_result = simulation.simulate();

			if ((current_result.points > best_result.points) || (current_result.points == best_result.points && current_result.len > best_result.len)) {
				best_result = current_result;
				new_best = true;
				move_number = 0;
				simulation.current_best = best_result.points;
			}

            auto t1 = tick();
			total_timing += timing(t0, t1);
			++simulation.run;
		}

        cerr << "Total simulations: " << simulation.run << ", it took: " << total_timing << " ms" << endl;

		if (!new_best) {
			++move_number;
		}

        auto found_valid_move = false;
        while(move_number < best_result.len && !found_valid_move) {
            if (floor(best_result.moves[move_number]) != Point(-1)) {
                cout << toInt(best_result.moves[move_number].x) << " " << toInt(best_result.moves[move_number].y) << endl;
                found_valid_move = true;
            } else {
                ++move_number;
            }
		}
    }

    return 0;
}