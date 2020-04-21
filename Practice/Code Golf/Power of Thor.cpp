// 242

#include <iostream>
using namespace std;
int main(){
    int X,Y,x,y;
    cin>>X>>Y>>x>>y;
    X-=x;
    Y-=y;
    for(;;)
        Y>0?cout<<"S",Y--:(Y<0?cout<<"N",Y++:0),
        X>0?cout<<"E",X--:(X<0?cout<<"W",X++:0),
        cout<<endl;
}

/* 189
#include <iostream>
using namespace std;int main(){int X,Y,x,y;cin>>X>>Y>>x>>y;X-=x;Y-=y; for(;;)Y>0?cout<<"S",Y--:(Y<0?cout<<"N",Y++:0),X>0?cout<<"E",X--:(X<0?cout<<"W",X++:0),cout<<endl;}
*/

// 243
#include <cstdio>
int main(){
    int X,Y,x,y;
    scanf("%d%d%d%d",&X,&Y,&x,&y);
    X-=x;
    Y-=y;
    for(;;)
        Y>0?printf("S"),Y--:(Y<0?printf("N"),Y++:0),
        X>0?printf("E"),X--:(X<0?printf("W"),X++:0),
        printf("\n");
}
