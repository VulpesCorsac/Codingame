# 1 - 251
I,S,N=input,str.split,int
e,v={},[]
o,l,u,z,s,t,a,r=[N(i)for i in S(I())]
for i in [1]*r:
 f,p=[N(j)for j in S(I())]
 e[f]=p
e[z]=s
while 1:
 f,p,d=S(I())
 f,p=N(f),N(p)
 print('BLOCK'*(f>=0 and((p>e[f]and d=='RIGHT')or(p<e[f]and d=='LEFT')))or'WAIT')

# 2 - 177
I,S,N=input,str.split,int
*_,f,p,t,a,e=S(I())
t={f:p,**dict([S(I())for _ in[1]*N(e)])}
while 1:
 f,p,d = S(I())
 print(('BLOCK','WAIT')['-'in p or(N(p)-N(t[f]))*d.find('I')<=0])

tst:
import sys
print(tmp,file=sys.stderr)