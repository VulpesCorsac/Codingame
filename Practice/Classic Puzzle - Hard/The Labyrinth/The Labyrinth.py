import sys
import math
from collections import deque


def validTile(tile):
    if tile[0] < 0 or tile[0] >= rows or tile[1] < 0 or tile[1] >= columns:
        return False
    return True


def neighboursTiles(tile):
    tile_row, tile_column = tile
    first_set = {(tile_row-1, tile_column), (tile_row+1, tile_column), (tile_row, tile_column-1), (tile_row, tile_column+1)}
    return {t for t in first_set if validTile(t)}


def firstStep(parent, start, finish):
    while parent[finish[0]][finish[1]] != start:
        finish = parent[finish[0]][finish[1]]
    return finish


def BFS(game_map, start, goal):
    colour   = [[0]          * columns for _ in range(rows)]
    distance = [[200*100*10] * columns for _ in range(rows)]
    parents  = [[None]       * columns for _ in range(rows)]
    tiles = deque([])

    colour[start[0]][start[1]] = 1
    distance[start[0]][start[1]] = 0

    tiles.append(start)

    while tiles:
        tile = tiles.popleft()
        forbidden_symbols = {'#'} 
        if goal == '?':
            forbidden_symbols.add('C')
        for neighbour in {x for x in neighboursTiles(tile) if game_map[x[0]][x[1]] not in forbidden_symbols}:
            if colour[neighbour[0]][neighbour[1]] == 0:
                colour[neighbour[0]][neighbour[1]] = 1
                distance[neighbour[0]][neighbour[1]] = distance[tile[0]][tile[1]] + 1
                parents[neighbour[0]][neighbour[1]] = tile
                tiles.append(neighbour)
                if game_map[neighbour[0]][neighbour[1]] == goal:
                    return firstStep(parents, start, neighbour)
        colour[tile[0]][tile[1]] = 2

    return None


def goal(game_map, start, go_back):
    if not go_back:
        go_to = BFS(game_map, start, '?')
        if go_to == None:
            return BFS(game_map, start, 'C')
        return go_to
    else:
        return BFS(game_map, start, 'T')


# rows: number of rows.
# columns: number of columns.
# alarm: number of rounds between the time the alarm countdown is activated and the time the alarm goes off.
rows, columns, alarm = [int(i) for i in input().split()]
back = False

while True:
    # kirk_row: row where Kirk is located.
    # kirk_column: column where Kirk is located.
    kirk_row, kirk_column = [int(i) for i in input().split()]
    game_map = [input() for _ in range(rows)]

    if game_map[kirk_row][kirk_column] == 'C':
        back = True

    go_to = goal(game_map, (kirk_row, kirk_column), back)

    if go_to[0] > kirk_row:
        print("DOWN")
    elif go_to[0] < kirk_row:
        print("UP")
    elif go_to[1] > kirk_column:
        print("RIGHT")
    else:
        print("LEFT")