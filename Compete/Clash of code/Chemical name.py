I=input
n,l,w = int(I()),'',{5:'methane',8:'ethane',11:'propane',14:'butane',17:'pentane'}
for _ in range(n):l+=I()
print(w.get(l.count('C')+l.count('H'),'NONE'))