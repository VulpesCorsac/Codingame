import sys
import math

def sign(x):
    if x > 0:
        return 1
    return -1


surface_n = int(input())
land = []
for i in range(surface_n):
    land_x, land_y = [int(j) for j in input().split()]
    land.append([land_x, land_y])
    if i > 0 and land[i][1] == land[i-1][1]:
        landing_y = land[i][1]
        landing_x0 = land[i-1][0]
        landing_x1 = land[i][0]
        landing_x = (landing_x0 + landing_x1) // 2
        landing_length = landing_x1 - landing_x0

while True:
    x, y, h_speed, v_speed, fuel, rotate, power = [int(i) for i in input().split()]

    if abs(landing_x - x) < (landing_length + 100) // 2:
        if abs(h_speed) > 10:
            print(sign(h_speed)*50, 4)
        else:
            print(0, (2, 4)[v_speed < -39])
    else:
        print(20*sign(landing_x-x)*sign(abs(h_speed)-55), 4)