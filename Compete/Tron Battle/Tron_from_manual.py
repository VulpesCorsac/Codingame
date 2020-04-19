# sighlty modified bot from https://vks.ai/2016-09-07-ai-challenge-in-78-lines


import sys
import time


debug_output = False


def print_to_err(*args):
    if debug_output:
        sys.stderr.write(', '.join([str(arg) for arg in args]) + "\n")


class Board():
    def __init__(self, board):
        self.board = board

    def show(self):
        return "\n".join([' '.join(map(str, x)) for x in self.board])


def move_to_dir(old, new):
    if old[0] < new[0]:
        return "RIGHT"
    if old[1] < new[1]:
        return "DOWN"
    if old[0] > new[0]:
        return "LEFT"
    return "UP"


def get_score(position):
    t1 = time.time()
    graphs = {i: {} for i in range(n_players)}
    to_be_occupied_tiles = set(x for x in occupied_tiles)
    order_of_turns = list(range(my_id, n_players)) + list(range(0, my_id))
#    print_to_err("order of turns", order_of_turns)
    move = 1
    board = [['.' for j in range(30)] for i in range(20)]

    while True:
        full = True
        moves = {}
        for bot_id in order_of_turns:
            for current_position in position[bot_id]:
                for neighbour_position in neighbours_tiles[current_position]:
                    if neighbour_position not in to_be_occupied_tiles or (neighbour_position in moves and move == 1):
                        full = False
                        to_be_occupied_tiles.add(neighbour_position)
                        moves[neighbour_position] = bot_id

        for cell, bot_id in moves.items():
            graphs[bot_id][cell] = move
            board[cell[1]][cell[0]] = bot_id
            
        if full:
            break

        position = [[position for position, bot_id in moves.items() if bot_id == i] for i in range(n_players)]
        move += 1
    
    my_tiles_reachable = len(graphs[my_id])
    enemy_tiles_reachable = sum([len(graphs[i]) for i in range(n_players) if i != my_id])

    distance_for_enemies = sum([sum(graphs[i].values()) for i in range(n_players) if i != my_id])
    print_to_err("time taken", time.time() - t1)
    return sum([my_tiles_reachable * 10000000, enemy_tiles_reachable * -100000, distance_for_enemies * 1]), Board(board)


neighbours_tiles = {}
for i in range(30):
    for j in range(20):
        neighbours = []
        if i < 29:
            neighbours.append((i+1, j))
        if i > 0:
            neighbours.append((i-1, j))
        if j < 19:
            neighbours.append((i, j+1))
        if j > 0:
            neighbours.append((i, j-1))
        neighbours_tiles[(i, j)] = neighbours


occupied_tiles = {}


while True:
    n_players, my_id = [int(i) for i in input().split()]
    current_moves = []

    for i in range(n_players):
        x0, y0, x1, y1 = [int(j) for j in input().split()]
        occupied_tiles[(x0, y0)] = i
        occupied_tiles[(x1, y1)] = i
        current_moves.append((x1, y1))

    for i, current_move in enumerate(current_moves):
        if current_move == (-1, -1):
            occupied_tiles = {k: v for k, v in occupied_tiles.items() if v != i}

    for p in range(n_players):
        x1, y1 = current_moves[p]
        if p == my_id:
            my_location = (x1, y1)
            scores = []
            print_to_err("my_location", my_location)
            print_to_err("neighbours_tiles", neighbours_tiles[my_location])
            for neighbour in neighbours_tiles[my_location]:
                if neighbour not in occupied_tiles:
                    players_positions = [[x] for x in current_moves.copy()]
                    players_positions[my_id] = [neighbour]

                    for i, current_move in enumerate(current_moves):
                        if current_move == (-1, -1):
                            players_positions[i] = []

                    score, board = get_score(players_positions)
                    scores.append((score, board, neighbour))

    best_score_move = sorted(scores, key=lambda x: x[0], reverse=True)[0]
    scoring = [scores[i][0] for i in range(len(scores))]
    print_to_err("scoring", scoring)
    print_to_err("best", best_score_move[0])
#    print_to_err("board")
#    print_to_err(best_score_move[1].show())
    print(move_to_dir(my_location, best_score_move[-1]))