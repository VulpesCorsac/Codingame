# 1 - 145
b=''.join('{:07b}'.format(ord(x))for x in input())
a=''
for i,q in enumerate(b):a+=[[' 0 0',' 00 0'][q<'1'],'0'][i and b[i]==b[i-1]]
print(a[1:])

# 2 - 162
import re
print(' '.join(['0',''][c[0]>'0']+'0 {}'.format(c.replace('1','0'))for c in re.findall(r'(1+|0+| )',''.join(bin(ord(c))[2:].zfill(7)for c in input()))))

# 3 - 142
r,m=str.replace,''.join('{:07b}'.format(ord(x))for x in input())
print(r(r(r((('00 ','0 ')[m[0]>'0']+m),'01','0 0 1'),'10','1 00 0'),'1','0'))
