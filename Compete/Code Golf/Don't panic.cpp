// 1 - 284
#import<bits/stdc++.h>
using namespace std;
main(){
    int z,s,f,p,c,e[9];
    string d;
    cin>>c>>c>>c>>z>>s>>c>>c>>p;
    for(c=0;c++<p;)cin>>f>>e[f];
    e[z]=s;
    for(;;)
        cin>>f>>p>>d,
        c=e[f],
        cout<<(c>p&d[0]=='L'|c<p&d[0]=='R'?"BLOCK\n":"WAIT\n");
}

//* 225
#import<bits/stdc++.h>
using namespace std;
main(){int z,s,f,p,c,e[9];string d;cin>>c>>c>>c>>z>>s>>c>>c>>p;for(c=0;c++<p;)cin>>f>>e[f];e[z]=s;for(;;)cin>>f>>p>>d,c=e[f],cout<<(c>p&d[0]=='L'|c<p&d[0]=='R'?"BLOCK\n":"WAIT\n");}
//*/