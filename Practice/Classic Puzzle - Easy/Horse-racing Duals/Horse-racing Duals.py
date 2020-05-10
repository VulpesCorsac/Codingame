n = int(input())
horses = []
for i in range(n):
    pi = int(input())
    horses.append(pi)

horses = sorted(horses)

ans = max(horses)
for i in range(1, len(horses)):
    ans = min(ans, horses[i] - horses[i-1])

print(ans)