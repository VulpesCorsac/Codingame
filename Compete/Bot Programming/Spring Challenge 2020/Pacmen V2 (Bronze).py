from collections import namedtuple, defaultdict
import sys
import math
import random

random.seed(42)

Coord = namedtuple('Coord', ['x', 'y'])
Unit  = namedtuple('Unit', ['id', 'me', 'pos', 'type'])

UP    = Coord(0, -1)
DOWN  = Coord(0, +1)
RIGHT = Coord(+1, 0)
LEFT  = Coord(-1, 0)

DIRECTIONS = (LEFT, UP, RIGHT, DOWN)

ROCK     = 0
PAPER    = 1
SCISSORS = 2

TARGET_EXPLORE = 0
TARGET_PELLET  = 1

def think(my_pacmen, turn):
    commands = []
    if canBoost:
        for pac in my_pacmen:
            commands.append(f'SPEED {pac.id}')
    else:
        for pac in my_pacmen:
            commands.append(f'MOVE {pac.id} {targets_tile[pac.id].x} {targets_tile[pac.id].y}')
    print("|".join(commands))
    turn += 1
    return turn


def dist(a, b):
    return abs(a.x - b.x) + abs(a.y - b.y)


def add(a, b):
    return Coord(a.x + b.x, a.y + b.y)


class Grid:
    def __init__(self, width, height):
        self.cells = []
        self.width = width
        self.height = height
        self.cells = [[None for x in range(width)] for y in range(height)]

    def add_cell(self, x, y, cell):
        self.cells[y][x] = cell

    def get_cell(self, x, y):
        if width > x >= 0 and height > y >= 0:
            return self.cells[y][x]
        return None


width, height = map(int, input().split())
to_explore = []


grid = Grid(width, height)
for y in range(height):
    line = input()
    for x in range(width):
        c = line[x]
        grid.add_cell(x,y,c)
        if c == '.' or c == ' ':
            to_explore.append(Coord(x,y))


targets_tile    = dict()
targets_target  = dict()
last_pos = dict()

turn = 0
canBoost = False
while True:
    canBoost = turn % 10 == 1
    
    my_pacmen = []
    opponent_pacmen = []
    pellets = []
    big_pellets = []  
    small_pellets = []

    my_score, opponent_score = [int(i) for i in input().split()]

    pacmen_count = int(input())
    for n in range(pacmen_count):
        inputs = input().split()
        id, me, x, y = map(int, inputs[:4])
        type = inputs[:5]
        pos = Coord(x,y)

        pac = Unit(id, me, pos, type)

        if me == 1:
            my_pacmen.append(pac)
        else:
            opponent_pacmen.append(pac)
  
    pellet_count = int(input())  
    for n in range(pellet_count):
        x, y, value = map(int, input().split())
        pellet = Coord(x,y)
        pellets.append(pellet)
        if value > 1:
            big_pellets.append(pellet)
        else:
            small_pellets.append(pellet)
        
    for pac in my_pacmen:
        deadlock = False
        if pac.id in targets_tile and targets_target[pac.id] == TARGET_PELLET:
            if targets_tile[pac.id] not in pellets:
                targets_tile.pop(pac.id)
        if pac.id in targets_tile and pac.pos != targets_tile[pac.id] and last_pos.get(pac.id, None) == pac.pos:
            targets_tile[pac.id] = None
            deadlock = True
        if not deadlock:
            if pac.id not in targets_tile or dist(pac.pos, targets_tile[pac.id]) < 5 or last_pos.get(pac.id, None) == pac.pos:
                targets_tile[pac.id] = None
                big_pellets.sort  (key=lambda p: dist(pac.pos, p))
                small_pellets.sort(key=lambda p: dist(pac.pos, p))
                big_pellet = 0
                for current_pellets in [big_pellets, small_pellets]:
                    big_pellet += 1
                    #print(current_pellets[:3], file=sys.stderr)
                    for current_pellet in current_pellets:
                        if not targets_tile[pac.id] or dist(pac.pos, targets_tile[pac.id]) > dist(pac.pos, current_pellet):
                            if dist(pac.pos, current_pellet) > 1 or current_pellet in big_pellets:
                                if not current_pellet in targets_tile.values():
                                    targets_tile[pac.id] = current_pellet
                                    break
                    if big_pellet == 1 and targets_tile[pac.id] and dist(pac.pos, targets_tile[pac.id]) < 5:
                        break
                targets_target[pac.id] = TARGET_PELLET
                print(pac.pos, '->', targets_tile[pac.id], file=sys.stderr)
        if pac.id not in targets_tile or not targets_tile[pac.id]:
            targets_tile[pac.id] = random.choice(to_explore)
            targets_target[pac.id] = TARGET_EXPLORE            
    last_pos[pac.id] = pac.pos

    turn = think(my_pacmen, turn)