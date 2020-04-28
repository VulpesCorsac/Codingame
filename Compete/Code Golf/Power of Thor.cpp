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
