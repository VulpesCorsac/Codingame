n = int(input())
for i in range(n):
    p0, p1 = [int(j) for j in input().split()]
    print(int(bin(p0)[2:])+int(bin(p1)[2:]))
