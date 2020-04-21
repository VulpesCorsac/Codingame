# 154
X,Y,x,y=[int(i)for i in input().split()]
while 1:m,y=(((["",y],["N",y-1])[y>Y]),["S",y+1])[y<Y];m,x=((([m,x],[m+"W",x-1])[x>X]),[m+"E",x+1])[x<X];print(m)