import sys

def toList(n):
    return [n // 1000, n // 100 % 10, n // 10 % 10, n % 10]


def getBullsAndCows(secret, guess):
    s = toList(secret)
    g = toList(guess)

    bulls = cows = 0
    bulls_list = []
    for i in range(4):
        if s[i] == g[i]:
            bulls += 1
            bulls_list.append(i)
    for item in reversed(bulls_list):
        s.pop(item)
        g.pop(item)

    for symbol in g:
        if symbol in s:
            cows += 1
            s.remove(symbol)

    return (bulls, cows)
        

n = int(input())
guesses = []
for i in range(n):
    guess, bulls, cows = input().split()
    guesses.append((int(guess), (int(bulls), int(cows))))
#    print(guess, bulls, cows, file=sys.stderr)

for guess in range(10000):
    accept = True
    for i in range(len(guesses)):
        if getBullsAndCows(guess, guesses[i][0]) != guesses[i][1]:
            accept = False
            break
    if accept:
        print("{:04d}".format(guess))
        break