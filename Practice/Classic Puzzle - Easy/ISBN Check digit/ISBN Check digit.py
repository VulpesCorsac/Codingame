import sys
import math


def is_valid_isbn(isbn):
    if len(isbn) not in (10, 13): 
        return False
    if 'X' in isbn[:-1]: 
        return False
    if len(isbn) == 10: 
        return is_valid_isbn10(isbn)
    return is_valid_isbn13(isbn)


def is_valid_isbn10(isbn):
    w = 10
    s = 0
    for d in isbn[:-1]:
        if not d.isdigit():
            return False
        s += w * int(d)
        w -= 1

    if s % 11 == 1: 
        return isbn[-1] == 'X'
    if isbn[-1] == 'X': 
        return False
    if s % 11 == 0: 
        return isbn[-1] == '0'
    return int(isbn[-1]) == 11 - s % 11


def is_valid_isbn13(isbn):
    w = 1
    s = 0
    for d in isbn[:-1]:
        if not d.isdigit():
            return False
        s += w * int(d)
        w ^= 2

    if isbn[-1] == 'X': 
        return False
    if s % 10 == 0: 
        return isbn[-1] == '0'
    return int(isbn[-1]) == 10 - s % 10


n = int(input())
invalid = list()
for i in range(n):
    isbn = input()
    print(isbn, file=sys.stderr)
    if not is_valid_isbn(isbn):
        invalid += [isbn]


print(len(invalid), 'invalid:')
for isbn in invalid:
    print(isbn)
