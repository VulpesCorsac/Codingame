import sys


class Thor:
    thor_x = thor_y = 0
    h = n = 0
    strike_width = 4
    giants = []
    action = "STRIKE"

    def __init__(self):
        self.thor_x, self.thor_y = [int(i) for i in input().split()]

    def scan(self):
        self.h, self.n = [int(i) for i in input().split()]
        self.giants.clear()
        for i in range(self.n):
            x, y = [int(j) for j in input().split()]
            self.giants.append({'x': x, 'y': y})

    def findCenter(self):
        center_x = center_y = 0
        for giant in self.giants:
            center_x += giant['x']
            center_y += giant['y']

        center_x /= len(self.giants)
        center_y /= len(self.giants)
        return [int(center_x), int(center_y)]

    def giantsInRangeOfStrike(self, x, y):
        closest_giants = []
        for giant in self.giants:
            if ((abs(giant['x'] - x) <= self.strike_width) and (abs(giant['y'] - y) <= self.strike_width)):
                closest_giants.append(giant)

        return closest_giants

    def giantIsTooClose(self, x, y):
        for giant in self.giants:
            if ((abs(giant['x'] - x) <= 1) and (abs(giant['y'] - y) <= 1)):
                return True
        return False

    def findMove(self, to_x, to_y):
        if self.thor_x == to_x and self.thor_y == to_y:
            self.action = "WAIT"
        else:
            self.action = ""
            if self.thor_y < to_y:
                self.thor_y += 1
                self.action = "S"
            if self.thor_y > to_y:
                self.thor_y -= 1
                self.action = "N"
            if self.thor_x < to_x:
                self.action += "E"
                self.thor_x += 1
            if self.thor_x > to_x:
                self.action += "W"
                self.thor_x -= 1

    def distance(self, first, second):
        return int(abs(first[0] - second[0]) + abs(first[1] - second[1]))
    
    def validCell(self, x, y):
        if 0 <= y < 18 and 0 <= x < 40:
            return True
        return False
        
    def runAway(self):
        invert_directions = {"S":[0,+1],"N":[0,-1],"E":[+1,0],"W":[-1,0],"SE":[+1,+1],"SW":[-1,+1],"NE":[+1,-1],"NW":[-1,-1]}
        options = []
        for direction_name, direction in invert_directions.items():
            x = self.thor_x + direction[0]
            y = self.thor_y + direction[1]
            if self.validCell(x, y) and not self.giantIsTooClose(x, y):
                options.append([direction_name, [len(self.giantsInRangeOfStrike(x, y)), [x, y]]])
        
        self.action = "STRIKE"
        best_move = [0, [0, 0]]
        best_distance = 0
        center = self.findCenter()
        for option in options:
            if (option[1][0] > best_move[0]) or (option[1][0] == best_move[0] and self.distance(option[1][1], center) > best_distance):
                best_move = option[1]
                self.action = option[0]
                best_distance = self.distance(best_move[1], center)

        if self.action != "STRIKE":
            self.thor_x = best_move[1][0]
            self.thor_y = best_move[1][1]

    def bestMove(self):
        center_x, center_y = self.findCenter()
        if not self.giantIsTooClose(self.thor_x, self.thor_y):
            self.findMove(center_x, center_y)
        else:
            self.runAway()

    def move(self):
        closest_giants = self.giantsInRangeOfStrike(self.thor_x, self.thor_y)
        if len(self.giants) == len(closest_giants):
            self.action = "STRIKE"
        else:
            self.bestMove()
        print(self.action)


thor = Thor()
while True:
    thor.scan()
    thor.move()