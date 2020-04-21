# 132
n,a=int(input()),[int(i)for i in input().split()]
r=(0,999)[n>0]
for i in a:r=(r,i)[abs(r)>abs(i)or(abs(r)==abs(i)and i>r)]
print(r)