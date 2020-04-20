import sys
import time


debug_output = True
debug_output_board = False


def print_to_err(*args):
    if debug_output:
        sys.stderr.write(' '.join([str(arg) for arg in args]) + "\n")


class Board():
    def __init__(self, board):
        self.board = board

    def show(self):
        return "\n".join([' '.join(map(str, x)) for x in self.board])


class Game():
    def __init__(self):
        self.neighbours_tiles = {}
        
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
                self.neighbours_tiles[(i, j)] = neighbours
                
        self.occupied_tiles = {}
    
    def move_to_dir(self, old, new):
        if old[0] < new[0]:
            return "RIGHT"
        if old[1] < new[1]:
            return "DOWN"
        if old[0] > new[0]:
            return "LEFT"
        return "UP"

    def get_score_single_step(self, position, analyze_id):
        t1 = time.time()
        graphs = {i: {} for i in range(self.n_players)}
        to_be_occupied_tiles = set(x for x in self.occupied_tiles)
        order_of_turns = list(range(analyze_id, self.n_players)) + list(range(0, analyze_id))
        move = 1
        board = [['.' for j in range(30)] for i in range(20)]

        while True:
            full = True
            moves = {}
            for bot_id in order_of_turns:
                for current_position in position[bot_id]:
                    for neighbour_position in self.neighbours_tiles[current_position]:
                        if neighbour_position not in to_be_occupied_tiles or (neighbour_position in moves and move == 1):
                            full = False
                            to_be_occupied_tiles.add(neighbour_position)
                            moves[neighbour_position] = bot_id

            for cell, bot_id in moves.items():
                graphs[bot_id][cell] = move
                board[cell[1]][cell[0]] = bot_id
                
            if full:
                break

            position = [[position for position, bot_id in moves.items() if bot_id == i] for i in range(self.n_players)]
            move += 1
        
        my_tiles_reachable = len(graphs[analyze_id])
        enemy_tiles_reachable = sum([len(graphs[i]) for i in range(self.n_players) if i != analyze_id])

        distance_for_enemies = sum([sum(graphs[i].values()) for i in range(self.n_players) if i != analyze_id])
        print_to_err("time taken:", time.time() - t1)
        return sum([my_tiles_reachable * 10000000, enemy_tiles_reachable * -100000, distance_for_enemies * 1]), Board(board)

    def get_scores(self, current_moves, analyze_id):
        location = current_moves[analyze_id]
        scores = []
        for neighbour in self.neighbours_tiles[location]:
            if neighbour not in self.occupied_tiles:
                players_positions = [[x] for x in current_moves.copy()]
                players_positions[analyze_id] = [neighbour]

                for i, current_move in enumerate(current_moves):
                    if current_move == (-1, -1):
                        players_positions[i] = []

                score, board = self.get_score_single_step(players_positions, analyze_id)
                scores.append([score, board, neighbour])
        scores = sorted(scores, key=lambda x: x[0], reverse=True)
        return scores

    def get_best_move(self, current_moves, analyze_id, analyse_cycles=0):
        if analyse_cycles == 0:
            return self.get_scores(current_moves, analyze_id)
        else:
            # Works worse!
            possible_moves = []
            for possible_move in self.neighbours_tiles[current_moves[analyze_id]]:
                if possible_move not in self.occupied_tiles:
                    possible_moves.append(possible_move)
            
            possible_moves_scores = list()
            for possible_move in possible_moves:
                temp_current_moves = current_moves
                for cycle in range(analyse_cycles):
                    order_of_turns = list(range(analyze_id, self.n_players)) + list(range(0, analyze_id))
                    
                    for id in order_of_turns:
                        if id == analyze_id and cycle == 0:
                            temp_current_moves[id] = possible_move
                        else:
                            scores = self.get_scores(temp_current_moves, id)
                            
                            if len(scores) == 0:
                                pass
                            else:
                                temp_current_moves[id] = scores[0][-1]
                possible_moves_scores.append(self.get_scores(temp_current_moves, id)[0])
                possible_moves_scores[-1][-1] = possible_move
            
            return possible_moves_scores
                
    def make_best_move(self, current_moves):
        scores = self.get_best_move(current_moves, self.my_id)
        
        my_location = current_moves[self.my_id]
        if len(scores) == 0:
            print(self.move_to_dir(my_location, my_location))
        else:
            best_score_move = scores[0]
            scoring = [scores[i][0] for i in range(len(scores))]
            print_to_err("scoring", scoring)
            print_to_err("best", best_score_move[0])
            
            if debug_output_board:
                print_to_err("board")
                print_to_err(best_score_move[1].show())
            print(self.move_to_dir(my_location, best_score_move[-1]))

    def make_move(self):
        self.n_players, self.my_id = [int(i) for i in input().split()]
        current_moves = []

        for i in range(self.n_players):
            x0, y0, x1, y1 = [int(j) for j in input().split()]
            self.occupied_tiles[(x0, y0)] = i
            self.occupied_tiles[(x1, y1)] = i
            current_moves.append((x1, y1))

        for i, current_move in enumerate(current_moves):
            if current_move == (-1, -1):
                occupied_tiles = {k: v for k, v in occupied_tiles.items() if v != i}
        
        self.make_best_move(current_moves)
    
    def play(self):
        while True:
            self.make_move()


game = Game()
game.play()