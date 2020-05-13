import sys
import math


class Player:
    class Zone:
        def __init__(self, lo, hi):
            self.update(lo, hi)

        def update(self, lo, hi):
            self.lo = lo
            self.hi = hi      
    
    def __init__(self):
        self.w, self.h = map(int, input().split())
        n = int(input())
        self.x, self.y = map(int, input().split())
        print(self.w, self.h, n, file=sys.stderr)
        print(self.x, self.y, file=sys.stderr)
        self.x_prev = self.y_prev = 0
        self.cold = self.Zone(0, self.w-1)
        self.warm = self.Zone(0, self.w-1)
        self.curr = self.Zone(0, self.w-1)
        self.first = True
        self.found_x = False
        self.outside = False
 
    def readBomb(self):
        self.bomb_dir = input()
        print(self.bomb_dir, file=sys.stderr)

    def found(self):
        temp_x, temp_y = self.x, self.y
        if not self.found_x:
            self.x = (self.curr.lo + self.curr.hi) // 2
            self.found_x = True
            self.curr.update(0, self.h-1)
            self.warm.update(self.curr.lo, self.curr.hi)
            self.cold.update(self.curr.lo, self.curr.hi)
            print('found: X case', file=sys.stderr)
        else:
            self.y = (self.curr.lo + self.curr.hi) // 2
            print('found: Y case', file=sys.stderr)
        self.first = True
        return self.x == temp_x and self.y == temp_y

    def get(self, val, lim):
        low, high = self.curr.lo, self.curr.hi
        give = low + high - val
        print('get:', low, high, give, file=sys.stderr)
        
        if (self.outside):
            print('get: outside', file=sys.stderr)
            if val == 0:
                print('get: zero value', file=sys.stderr)
                give = give // 2
            elif val == lim:
                print('get: limit value', file=sys.stderr)
                give = (lim + give) // 2

        if give == val:
            print('get: give is value', file=sys.stderr)
            give = val + 1
        
        self.outside = False
        if give < 0 or give > lim:
            print('get: give out of bonds', file=sys.stderr)
            give = max(0, min(lim, give))
            self.outside = True

        middle = (give + val    ) // 2
        lower  = (give + val - 1) // 2
        higher = (give + val + 1) // 2
        
        if give > val:
            print('get: give > val', file=sys.stderr)
            self.cold.update(low, lower)
            self.warm.update(higher, high)
        elif give < val:
            print('get: give < val', file=sys.stderr)
            self.warm.update(low, lower)
            self.cold.update(higher, high)

        return give

    def move(self):
        self.x_prev, self.y_prev = self.x, self.y

        if self.bomb_dir == 'WARMER':
            self.curr = self.warm
            print('move: warm', file=sys.stderr)
        elif self.bomb_dir == 'COLDER':
            self.curr = self.cold
            print('move: cold', file=sys.stderr)
        elif self.bomb_dir == 'SAME':
            if not self.first and not self.found():
                print('move: Return in switch', file=sys.stderr)
                return

        if self.curr.lo >= self.curr.hi and not self.found():
            print('move: Return not in switch', file=sys.stderr)
            return
        
        self.first = False
        if not self.found_x: 
            print('move: X case', file=sys.stderr)
            self.x = self.get(self.x, self.w-1)
        else:
            print('move: Y case', file=sys.stderr)
            self.y = self.get(self.y, self.h-1)

    def play(self):
        while True:
            self.readBomb()
            self.move()
            print(self.x, self.y)
            print('curr:', self.curr.lo, self.curr.hi, file=sys.stderr)
            print('cold:', self.cold.lo, self.cold.hi, file=sys.stderr)
            print('warm:', self.warm.lo, self.warm.hi, file=sys.stderr)


player = Player()
player.play()

