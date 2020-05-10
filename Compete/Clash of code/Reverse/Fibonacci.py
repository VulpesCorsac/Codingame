n = int(input())
if n == 1:
    print(0)
else:
    r = [0, 1]
    while n > 2:
        r += [r[-1] + r[-2]]
        n -= 1

    print(" ".join([str(i) for i in r]))