// 1 - 242
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

// 1.1 - 222
#include <iostream>
using namespace std;
int main(){
    int X,Y,x,y;
    cin>>X>>Y>>x>>y;
    X-=x;
    Y-=y;
    for(;;)
        Y>0?cout<<"S",Y--:0,
        X>0?cout<<"E",X--:(X<0?cout<<"W",X++:0),
        cout<<endl;
}

/* 169
#include <iostream>
using namespace std;int main(){int X,Y,x,y;cin>>X>>Y>>x>>y;X-=x;Y-=y; for(;;)Y>0?cout<<"S",Y--:0,X>0?cout<<"E",X--:(X<0?cout<<"W",X++:0),cout<<endl;}
*/

// 1.2 - 199
#include<iostream>
#define O(x)std::cout<<(x)
main(){
    int X,Y,x,y;
    std::cin>>X>>Y>>x>>y;
    for(;;)
        O(Y>y?(y++,"S"):""),
        O(X>x?(x++,"E"):X<x?(x--,"W"):""),
        O("\n");
}

/* 156 
#include<iostream>
#define O(x)std::cout<<(x)
main(){int X,Y,x,y;std::cin>>X>>Y>>x>>y;for(;;)O(Y>y?(y++,"S"):""),O(X>x?(x++,"E"):X<x?(x--,"W"):""),O("\n");}
*/

// 1.3 - 189
#include<iostream>
#define O(x)std::cout<<(#x)
main(){
    int X,Y,x,y;
    std::cin>>X>>Y>>x>>y;
    for(;;)
        Y>y?O(S),Y--:0,
        X>x?O(E),X--:(X<x?O(W),X++:0),
        O(\n);
}

/* 146
#include<iostream>
#define O(x)std::cout<<(#x)
main(){int X,Y,x,y;std::cin>>X>>Y>>x>>y;for(;;)Y>y?O(S),Y--:0,X>x?O(E),X--:(X<x?O(W),X++:0),O(\n);}
*/