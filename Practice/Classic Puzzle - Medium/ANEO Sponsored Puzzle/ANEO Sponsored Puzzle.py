import sys
import math

speed = int(input())
light_count = int(input())
lights = []
for i in range(light_count):
    distance, duration = [int(j) for j in input().split()]
    lights.append([distance, duration])


def isRed(speed, distance, duraion):
    return 18 * distance % (5 * speed * 2 * duraion) >= 5 * speed * duraion


for s in range(speed, 0, -1):
    red = False

    for i in range(len(lights)):
        if isRed(s, lights[i][0], lights[i][1]):
            red = True
            break

    if not red:
        print(s)
        break