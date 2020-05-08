n=int(input())
for i in range(n):
 l=input()
 for j in range(10):
  if str(j) not in l:
   print(j)