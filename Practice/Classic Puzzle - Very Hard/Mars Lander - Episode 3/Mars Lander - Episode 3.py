import sys


surface_n = int(input())  # the number of points used to draw the surface of Mars.
land = []
landing_zone = -1
hight = -1
test_case = -1
move_left = False
for i in range(surface_n):
    land_x, land_y = [int(j) for j in input().split()]
    land.append([land_x, land_y])
    hight = max(hight, land_y)
    if i > 0 and land[i][1] == land[i-1][1]:
        landing_zone = i-1


while True:
    x, y, h_speed, v_speed, fuel, rotate, power = [int(i) for i in input().split()]
    if test_case == -1:
        test_case = h_speed != 0
        print(test_case, file=sys.stderr)
    if test_case:
        if y - land[landing_zone][1] < 800:
            if v_speed < -40:
                print("Use more fuel and land", file=sys.stderr)
                print("0 4")
            else:
                print("Use less fuel and land", file=sys.stderr)
                print("0 3")
        elif x < land[landing_zone+1][0]:
            print("Stabilize horizontal", file=sys.stderr)
            print("-45 4")
        elif v_speed <= -20:
            print("Use more fuel and stand still", file=sys.stderr)
            print("0 4")
        elif v_speed <= -12:
            print("Use less fuel and stand still", file=sys.stderr)
            print("0 2")
        else:
            print("Moving to cave", file=sys.stderr)
            print("45 4")
    else:
        print(move_left, file=sys.stderr)
        if v_speed < -45 or y <= 1135:
            print("Land", file=sys.stderr)
            print("0 4")
        elif x <= land[landing_zone][0]:
            print("Move right", file=sys.stderr)
            print("-32 3")
        elif v_speed == 0 and y > hight:
            print("Stabilize height", file=sys.stderr)
            print("0 3")
        elif v_speed < 0 or y < hight:
            print("Get up", file=sys.stderr)
            print("0 4")
        elif v_speed >= 12 or move_left:
            print("Move left", file=sys.stderr)
            print("45 4")
            move_left = True
        else:
            print("Stand still full power", file=sys.stderr)
            print("0 4")