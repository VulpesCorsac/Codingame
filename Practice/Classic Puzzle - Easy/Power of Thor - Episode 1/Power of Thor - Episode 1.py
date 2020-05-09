# 1 - 154
X,Y,x,y=[int(i)for i in input().split()]
while 1:m,y=(((["",y],["N",y-1])[y>Y]),["S",y+1])[y<Y];m,x=((([m,x],[m+"W",x-1])[x>X]),[m+"E",x+1])[x<X];print(m)

# 2 - 115
X,Y,x,y=map(int,input().split())
while 1:
 d=["","S"][Y>y];y+=Y>y
 if X>x:d+="E";x+=1
 if X<x:d+="W";x-=1
 print(d)
