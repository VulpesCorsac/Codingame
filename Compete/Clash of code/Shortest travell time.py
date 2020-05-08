n=int(input())
e=[]
for i in range(n):
 s,d=[int(j)for j in input().split()]
 e.append(round(d/(s/60)))
print(min(e))