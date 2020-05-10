import sys
import math

route = list(input())
d = dict()
pos = (0, 0)

d[pos] = 1
for item in route:
    if item == '>':
        next_pos = (pos[0], pos[1]+1)
    elif item == '<':
        next_pos = (pos[0], pos[1]-1)
    elif item == '^':
        next_pos = (pos[0]+1, pos[1])
    elif item == 'v':
        next_pos = (pos[0]-1, pos[1])
    if next_pos in d.keys():
        d[next_pos] += 1
    else:
        d[next_pos] = 1
    pos = next_pos

print(max(d.values()))