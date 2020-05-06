guesses = [input().split() for i in range(int(input()))]

for secret in range(10000):
    secret = str(secret).zfill(4)
    matching = True
    for guess_number, guess_bulls, guess_cows in guesses:
        bulls = sum(x == y for x, y in zip(guess_number, secret))
        cows = sum(min(secret.count(digit), guess_number.count(digit)) for digit in set(secret)) - bulls
        if bulls != int(guess_bulls) or cows != int(guess_cows): 
            matching = False
    if matching:
        print(secret)