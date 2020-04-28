# 1 - 132
n,a=int(input()),[int(i)for i in input().split()]
r=(0,999)[n>0]
for i in a:r=(r,i)[abs(r)>abs(i)or(abs(r)==abs(i)and i>r)]
print(r)

# 2 - 94
s,i=sorted,input
r,a=i(),[int(j)for j in i().split()]
if a:r=s(s(a)[::-1],key=abs)[0]
print(r)

# 3 - 73
input()
print(min(input().split(),key=lambda x:abs(int(x)-.1),default=0))