import sys
import math


def strToRad(data):
    return float(data.replace(",", ".")) * 3.14 / 180


class Defibrillator:
    def __init__(self, data=None):
        if data:  
            data = data.split(";")
            
            self.id = data[0]
            self.name = data[1]
            self.address = data[2]
            self.phone = data[3]
            self.longitude = strToRad(data[4])
            self.latitude  = strToRad(data[5])
        else:
            self.name = ""


def distance(from_longitude, from_latitude, to_longitude, to_latitude):
    x = (to_longitude - from_longitude) * math.cos(0.5 * (from_latitude + to_latitude))
    y = (to_latitude - from_latitude)
    
    return (x**2 + y**2)**0.5 * 6371


def distance_to_defibrillator(defibrillator):
    return distance(usr_longitude, usr_latitude, defibrillator.longitude, defibrillator.latitude)


nearest = Defibrillator()
usr_longitude = strToRad(input())
usr_latitude  = strToRad(input())
n = int(input())
for i in range(n):
    current = Defibrillator(input())
    
    if nearest.name == "" or distance_to_defibrillator(current) < distance_to_defibrillator(nearest):
        nearest = current;

# Write an answer using print
# To debug: print("Debug messages...", file=sys.stderr)

print(nearest.name)