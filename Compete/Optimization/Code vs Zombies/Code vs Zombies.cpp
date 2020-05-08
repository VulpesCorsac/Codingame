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

constexpr float PI = 3.1416;
constexpr float EPS = 1E-3;

constexpr size_t max_zombies = 100;
constexpr size_t max_humans  = 100;

constexpr float max_evaluation_time = 140;
constexpr size_t max_moves   = 100;
constexpr size_t MAX_SIM     = 1000000;

constexpr int PLAYER_ID = -1;
constexpr int TILE_ID = -2;
constexpr int EMPTY_ZOMBIE = -4;
constexpr int EMPTY_HUMAN = -5;

constexpr float player_shoot_range = 2000;
constexpr float player_move_range  = 1000;
constexpr float zombie_move_range  = 400;

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
    return static_cast < int > (floorf(x));
}

template < typename T >
inline float randomFloat(T max_value) noexcept {
    return static_cast < float > (max_value) * rand() / RAND_MAX;
}

constexpr size_t max_fibonacci = 40;
int fibonacci[max_fibonacci];

void fillFibonacci() noexcept {
    fibonacci[0] = fibonacci[1] = 1;
    for (size_t i = 2; i < max_fibonacci; ++i) {
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

public:
    float x = 0;
    float y = 0;
};

inline auto distance(const Point& p1, const Point& p2) {
    return sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
}

Point area(16000, 900);

class GameObject {
public:
    void resetCoordinates() noexcept {
        position = starting_position;
    }

public:
    int id = 0;
    Point starting_position;
    Point position;
};

void initTile(GameObject *tile, int x, int y) noexcept {
	tile->id = TILE_ID;
	tile->position.x = x;
	tile->position.y = y;
}

GameObject *randomTileInArea() {
	GameObject *rt = new(GameObject);
	initTile(rt, randomInt(area.x), randomInt(area.y));
	return rt;
}

GameObject *randomTileInCircle(Point center, float radius) {
	float angle = randomFloat(1.0f) * 2 * PI;
	float x = cosf(angle) * radius, y = sinf(angle) * radius;
	GameObject *rt = new(GameObject);
	initTile(rt, fabsf(floorf(x + center.x)), fabsf(floorf(y + center.y)));
	return rt;
}

class Human : public GameObject {

};

class Zombie {
public:
	GameObject gocomp;
	bool arrived;
	GameObject *target;
};

class Player {
public:
	GameObject gocomp;
	bool arrived;
	bool targetingZombie;
	Zombie *target_zombie;
	GameObject *target_tile;
};

class Result {
public:
    int points;
    Point moveList[max_moves];
    int len;
};

size_t  humans_count = 0;
size_t zombies_count = 0;

Human  humans [max_humans ];
Zombie zombies[max_zombies];

/// Simulation

Player simulation_player;
Zombie simulation_player_target_cpy;
Human  simulation_humans [max_humans+1];
Zombie simulation_zombies[max_zombies];

bool simulation_failure = false;
bool simulation_zombies_all_dead = false;
bool simulation_zombies_died_this_turn = false;
bool simulation_player_target_died_this_turn = false;

int simulation_points = 0;
int simulation_current_best = 0;
size_t simulation_moves_count = 0;
size_t simulation_zombies_count = 0;
size_t simulation_humans_count = 0;

int simulation_starting_random_moves_number = -1;
int simulation_starting_random_moves_number_max = 3;

float total_timing = 0;
Result bestResults, tmpResults;
bool newBest = false;

Player player;

class Simulation {
public:
    void setup() noexcept {
        simulation_player = player;

        for (size_t i = 0; i < zombies_count; i++) {
            simulation_zombies[i] = zombies[i];
        }

        for (size_t i = 0; i < humans_count; i++) {
            simulation_humans[i] = humans[i];
        }

        simulation_failure = false;
        simulation_zombies_all_dead = false;
        simulation_zombies_died_this_turn = false;
        simulation_player_target_died_this_turn = false;

        simulation_points = 0;
        simulation_moves_count = 0;
        simulation_zombies_count = zombies_count;
        simulation_humans_count = humans_count;

        simulation_starting_random_moves_number = 0;
        simulation_starting_random_moves_number_max = 3;
    }

    int maxScore() const noexcept {
        int result = 0;
        int human_points = 10 * sqr(simulation_humans_count);
        for (size_t i = 0; i < simulation_zombies_count; i++) {
            auto points = human_points;
            if (simulation_zombies_count > 1) {
                points *= fibonacci[i + 1];
            }
            result += points;
        }

        return result;
    }

    void computePlayerTarget() noexcept {
        Zombie* zombies_do_not_target_player[max_zombies];

        if (simulation_starting_random_moves_number > 0) {
            simulation_player.target_tile = randomTileInArea();
            simulation_player.targetingZombie = false;
            --simulation_starting_random_moves_number;
        } else {
            size_t len = 0;
            for (size_t i = 0; i < simulation_zombies_count; i++) {
                if (simulation_zombies[i].target != nullptr && simulation_zombies[i].target->id != PLAYER_ID) {
                    zombies_do_not_target_player[len++] = &simulation_zombies[i];
                }
            }

            simulation_player.target_zombie = (len > 0) ? zombies_do_not_target_player[randomInt(len)] : &simulation_zombies[randomInt(simulation_zombies_count)];
            simulation_player.arrived = false;
            simulation_player.targetingZombie = true;
        }
    }

    auto getPlayerTargetId() noexcept {
        return simulation_player.targetingZombie ? simulation_player.target_zombie->gocomp.id : simulation_player.target_tile->id;
    }

public:
    int run = 0;
};

Simulation simulation;

/// Legacy

#define isMoveValid(move) (int)floorf(move.x) != -1 && (int)floorf(move.y) != -1

#define printErr(fmt, val) fprintf(stderr, fmt, val)
#define printMove(x, y) printf("%d %d\n", (int)floorf(x), (int)floorf(y));

Result simulate();
void turn(Point moveHistory[max_moves]);
void findZombieTarget(Zombie *zombie);
void movePlayer(Player *player);
void moveZombie(Zombie *zombie);
bool nextPosZombie(Zombie zombie, Point *pos_out);
bool nextPosPlayer(Player player, Point *pos_out);
void evaluate();
void zombiesEat();
int eat(Zombie zombie);
Point getPlayerDestination();

inline void init() noexcept {
	srand(time(nullptr));
    fillFibonacci();
}

int main() {
    init();

	for (size_t i = 0; i < max_moves; i++) {
		bestResults.moveList[i].x = bestResults.moveList[i].y = -1;
	}
	bestResults.points = 0;

    int move_number = 0;
	while (true) {
		cin >> player.gocomp.position.x >> player.gocomp.position.y;
		player.gocomp.id = PLAYER_ID;
		player.gocomp.starting_position = player.gocomp.position;
		player.arrived = false;
		player.target_zombie = nullptr;
		player.targetingZombie = false;
		player.target_tile = nullptr;

		cin >> humans_count;
        for (size_t i = 0; i < humans_count; i++) {
            auto& human = humans[i];
            cin >> human.id >> human.position.x >> human.position.y;
            human.starting_position = human.position;
		}

		cin >> zombies_count;
        for (size_t i = 0; i < zombies_count; i++) {
            auto& zombie = zombies[i];
            cin >> zombie.gocomp.id >> zombie.gocomp.position.x >> zombie.gocomp.position.y;
            int t;
            cin >> t >> t;
            zombie.gocomp.starting_position = zombie.gocomp.position;
			zombie.arrived = false;
			zombie.target = nullptr;
		}

        newBest = false;
        simulation.run = 0;
        total_timing = 0;

		while (total_timing < max_evaluation_time) {
            auto t0 = tick();

			simulation.setup();
			auto zombiesBefore = simulation_zombies_count;
			tmpResults = simulate();
			auto zombiesAfter = simulation_zombies_count;

			if (tmpResults.points > bestResults.points || (tmpResults.points == bestResults.points && tmpResults.len > bestResults.len)) {
				bestResults = tmpResults;
				newBest = true;
				move_number = 0;
				simulation_current_best = bestResults.points;
			}

            auto t1 = tick();
			total_timing += timing(t0, t1);
			simulation.run++;
		}

        cerr << "Total simulations: " << simulation.run << ", it took: " << total_timing << " ms" << endl;

		if (!newBest) {
			move_number++;
		}

        bool foundValidMove = false;
        while(move_number < bestResults.len && !foundValidMove) {
            if (isMoveValid(bestResults.moveList[move_number])) {
                cout << toInt(bestResults.moveList[move_number].x) << " " << toInt(bestResults.moveList[move_number].y) << endl;
                foundValidMove = true;
            } else {
                move_number++;
            }
		}

		if (!foundValidMove) {
			for (int k = 0; k < bestResults.len; k++) {
				fprintf(stderr, "move %d is (%f, %f)\n", k, bestResults.moveList[k].x, bestResults.moveList[k].y);
			}
		}
    }

    return 0;
}

Result simulate() {
	Result simResult;

	for (size_t i = 0; i < max_moves; i++) {
		simResult.moveList[i].x = -1;
		simResult.moveList[i].y = -1;
	}
	simResult.points = 0;

	simulation_starting_random_moves_number = randomInt(simulation_starting_random_moves_number_max + 1);

	simulation.computePlayerTarget();

	while (!simulation_zombies_all_dead && !simulation_failure && simulation_moves_count < max_moves) {
		if ((simulation.maxScore() + simulation_points) < simulation_current_best) {
			simulation_failure = true;
		}
		turn(simResult.moveList);
	}

	if (simulation_zombies_all_dead && !simulation_failure) {
		simResult.points = simulation_points;
		simResult.len = simulation_moves_count;
	}

	return simResult;
}

void turn(Point moveHistory[max_moves]) {
	for (size_t i = 0; i < simulation_zombies_count; i++) {
		findZombieTarget(&simulation_zombies[i]);
		moveZombie(&simulation_zombies[i]);
	}

	moveHistory[simulation_moves_count] = getPlayerDestination();
	simulation_moves_count++;
	movePlayer(&simulation_player);

	evaluate();

	zombiesEat();

	if ((simulation_humans_count) > 0 && (simulation_zombies_count > 0)) {
		if (simulation_player.arrived || simulation_player_target_died_this_turn) {
			simulation.computePlayerTarget();
			simulation_player_target_died_this_turn = false;
		}
	} else {
		simulation_failure = (simulation_humans_count <= 0);
		simulation_zombies_all_dead = simulation_zombies_count <= 0;
	}
}

void findZombieTarget(Zombie *zombie) {
	float minDist = INFINITY, tmpDist;
    bool targetFound = false;

	zombie->arrived = false;

	tmpDist = distance(zombie->gocomp.position, simulation_player.gocomp.position);
	if (tmpDist < minDist) {
		zombie->target = &(simulation_player.gocomp);
		targetFound = true;
		minDist = tmpDist;
	}

	for (size_t i = 0; i < simulation_humans_count; i++) {
		tmpDist = distance(zombie->gocomp.position, simulation_humans[i].position);
		if (tmpDist < minDist) {
			zombie->target = &simulation_humans[i];
			targetFound = true;
			minDist = tmpDist;
		}
	}

	//printErr("zombie is at %f distance from target\n", distance(zombie->gocomp.position, zombie->target->position));
}

bool nextPosPlayer(Player player, Point *pos_out) {
	Point dst;
	float dft, t;
	bool arrived = false;

	if (player.target_zombie != nullptr || player.target_tile != nullptr) {

		/*if (player->target->gocomp.id == PLAYER_ID) {
			player->arrived = true;
			return player->gocomp.position;
		} */
		dst = getPlayerDestination();
		dft = distance(player.gocomp.position, dst);

		if (floorf(dft) <= player_move_range) {
			arrived = true;
			pos_out->x = dst.x;
			pos_out->y = dst.y;
		} else {
			t = player_move_range / dft;
			pos_out->x = player.gocomp.position.x + floorf(t * (dst.x - player.gocomp.position.x));
			pos_out->y = player.gocomp.position.y + floorf(t * (dst.y - player.gocomp.position.y));
		}
	} else {
		//fprintf(stderr, "Move error\n");
		pos_out->x = -1.0f;
		pos_out->y = -1.0f;
	}

	return arrived;
}

bool nextPosZombie(Zombie zombie, Point *pos_out) {
	bool arrived = false;

	if (zombie.target != nullptr) {
		float dft = distance(zombie.gocomp.position, zombie.target->position), t;

		if (floorf(dft) <= zombie_move_range) {
			arrived = true;
			pos_out->x = zombie.target->position.x;
			pos_out->y = zombie.target->position.y;
		} else {
			t = zombie_move_range / dft;
			pos_out->x = zombie.gocomp.position.x + floorf(t * (zombie.target->position.x - zombie.gocomp.position.x));
			pos_out->y = zombie.gocomp.position.y + floorf(t * (zombie.target->position.y - zombie.gocomp.position.y));
		}
	} else {
		//fprintf(stderr, "Zombie move error\n");
		pos_out->x = -1.0f;
		pos_out->y = -1.0f;
	}

	return arrived;
}

void movePlayer(Player *player) {
	player->arrived = nextPosPlayer(*player, &player->gocomp.position);
}

void moveZombie(Zombie *zombie) {
	zombie->arrived = nextPosZombie(*zombie, &zombie->gocomp.position);
}

int zombiesInRangeOfPlayer(Zombie zombiesInRange[max_zombies]) {
	int len = 0;
	float dx, dy;

	for (size_t i = 0; i < simulation_zombies_count; i++) {
		dx = simulation_zombies[i].gocomp.position.x - simulation_player.gocomp.position.x;
		dy = simulation_zombies[i].gocomp.position.y - simulation_player.gocomp.position.y;

		if (floorf(sqrtf(dx * dx + dy * dy)) <= player_shoot_range) {
			zombiesInRange[len] = simulation_zombies[i];
			len++;
		}
	}

	return len;
}

void evaluate() {
	bool found;
	int i, j, k, tmpPoints;
	int humanNum = simulation_humans_count;
	int humanPoints = 10 * humanNum * humanNum;
	Zombie killableZombies[max_zombies];
	int killableZombiesLen = zombiesInRangeOfPlayer(killableZombies);

	int tmpId = (simulation_player.targetingZombie) ? simulation_player.target_zombie->gocomp.id : EMPTY_ZOMBIE;

	for (i = 0; i < killableZombiesLen; i++) {
		tmpPoints = humanPoints;

		if (killableZombiesLen > 1) {
			tmpPoints *= fibonacci[i + 1];
		}
		simulation_points += tmpPoints;

		if (killableZombies[i].gocomp.id == tmpId) {
			simulation_player_target_died_this_turn = true;
		}

		found = false;
		for (j = simulation_zombies_count - 1; j >= 0 && !found; j--) {
			if (simulation_zombies[j].gocomp.id == killableZombies[i].gocomp.id) {
				simulation_zombies_count--;
				found = true;

				// eliminate the j-th zombie
				for (k = j; (k + 1) < max_zombies; k++) {
					simulation_zombies[k] = simulation_zombies[k + 1];
				}
			}
		}
	}

	if (killableZombiesLen > 0) {
		found = false;
		for (i = 0; i < simulation_zombies_count && !found; i++) {
			if (simulation_zombies[i].gocomp.id == tmpId) {
				simulation_player.target_zombie = &simulation_zombies[i];
			}
		}
	}
}

bool _zombieArrivedAtTarget(Zombie zombie) {
	return (int)zombie.gocomp.position.x == (int)zombie.target->position.x && (int)zombie.gocomp.position.y == (int)zombie.target->position.y;
}

void zombiesEat() {
    int zombieTargetIdTmp[max_zombies];
    bool foundHuman;

	for (size_t i = 0; i < simulation_zombies_count; i++) {
        zombieTargetIdTmp[i] = simulation_zombies[i].target->id;
	}

	for (size_t i = 0; i < simulation_zombies_count; i++) {
		foundHuman = false;
		// if you can eat your target
		if (_zombieArrivedAtTarget(simulation_zombies[i])) {
			for (size_t j = 0; j < simulation_humans_count && !foundHuman; j++) {
				if (simulation_humans[j].id == zombieTargetIdTmp[i]) {
					for (size_t k = j; (k + 1) < max_humans; k++) {
						simulation_humans[k] = simulation_humans[k + 1];
					}
					foundHuman = true;
					simulation_humans_count--;
				}
			}
		}
	}

	for (size_t i = 0; i < simulation_zombies_count; i++) {
		for (size_t j = 0; j < simulation_humans_count; j++) {
			if (simulation_humans[j].id == zombieTargetIdTmp[i]) {
				simulation_zombies[i].target = &simulation_humans[j];
			}
		}
	}
}

int eat(Zombie zombie) {
	bool found = false;
	int humanId = -3;

	// if you can eat your target
	if (_zombieArrivedAtTarget(zombie)) {
		for (size_t i = 0; i < simulation_humans_count && !found; i++) {
			if (simulation_humans[i].id == zombie.target->id) {
				for (size_t j = i; (j + 1) < max_humans; j++)
					simulation_humans[j] = simulation_humans[j + 1];
				found = true;
				simulation_humans_count--;
			}
		}
	}

	return humanId;
}

Point getPlayerDestination() {
    Zombie *target;
    Point dst;
    if (simulation_player.targetingZombie) {
        target = simulation_player.target_zombie;
        nextPosZombie(*target, &dst);
        return dst;
    } else {
        return simulation_player.target_tile->position;
    }
}
