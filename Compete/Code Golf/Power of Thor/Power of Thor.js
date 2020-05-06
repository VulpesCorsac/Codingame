// 1 - 100
[X,Y,x,y]=readline().split(' ');
for(;;){
    print((Y>y++?"S":"")+(X>x++?"E":X<--x?(x--,"W"):""))
}

/* 94
[X,Y,x,y]=readline().split(' ');
for(;;){print((Y>y++?"S":"")+(X>x++?"E":X<--x?(x--,"W"):""))}
*/

// 1.1 - 87
for([X,Y,x,y]=readline().split` `;;)
    print((y++<Y?'S':'')+(x--<X?'E':x++>X?'W':''))

/* 82
for([X,Y,x,y]=readline().split` `;;)print((y++<Y?'S':'')+(x--<X?'E':x++>X?'W':''))
*/