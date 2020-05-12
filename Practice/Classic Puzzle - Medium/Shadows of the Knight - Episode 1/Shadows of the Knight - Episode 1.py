import sys
import math

# w: width of the building.
# h: height of the building.
w, h = [int(i) for i in input().split()]
n = int(input())  # maximum number of turns before game over.
x0, y0 = [int(i) for i in input().split()]

minx, miny, maxx, maxy = 0, 0, w - 1, h - 1

while True:
    bomb_dir = input()  # the direction of the bombs from batman's current location (U, UR, R, DR, D, DL, L or UL)

    if bomb_dir.find("U") > -1:
        maxy = y0 - 1
    if bomb_dir.find("D") > -1:
        miny = y0 + 1
    if bomb_dir.find("L") > -1:
        maxx = x0 - 1
    if bomb_dir.find("R") > -1:
        minx = x0 + 1

    x0 = (minx + maxx) // 2
    y0 = (miny + maxy) // 2
    
    print("{0} {1}".format(x0, y0))