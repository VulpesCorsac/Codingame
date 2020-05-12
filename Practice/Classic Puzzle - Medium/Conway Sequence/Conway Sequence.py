import sys
import math

r = int(input())
l = int(input())

prev = [r]
for _ in range(1, l):
    next = list()

    cnt, element = 0, -1
    for item in prev:
        if item != element:
            if cnt > 0:
                next.append(cnt)
                next.append(element)
            cnt = 1
        else:
            cnt += 1
        element = item
    next.append(cnt)
    next.append(element)
    prev = next

    print(prev, file=sys.stderr)


print(' '.join(str(item) for item in prev))
