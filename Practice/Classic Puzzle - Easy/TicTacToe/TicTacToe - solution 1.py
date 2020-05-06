def canWin(c):
    return c.count('O') == 2 and c.count('.') == 1


lines = list()
for i in range(3):
    lines.append(list(input()))


found = False
if not found and canWin([lines[0][0],lines[0][1],lines[0][2]]):
    lines[0][0]=lines[0][1]=lines[0][2]='O'
    found = True
if not found and canWin([lines[1][0],lines[1][1],lines[1][2]]):
    lines[1][0]=lines[1][1]=lines[1][2]='O'
    found = True
if not found and canWin([lines[2][0],lines[2][1],lines[2][2]]):
    lines[2][0]=lines[2][1]=lines[2][2]='O'
    found = True
if not found and canWin([lines[0][0],lines[1][0],lines[2][0]]):
    lines[0][0]=lines[1][0]=lines[2][0]='O'
    found = True
if not found and canWin([lines[0][1],lines[1][1],lines[2][1]]):
    lines[0][1]=lines[1][1]=lines[2][1]='O'
    found = True
if not found and canWin([lines[0][2],lines[1][2],lines[2][2]]):
    lines[0][2]=lines[1][2]=lines[2][2]='O'
    found = True
if not found and canWin([lines[0][0],lines[1][1],lines[2][2]]):
    lines[0][0]=lines[1][1]=lines[2][2]='O'
    found = True
if not found and canWin([lines[0][2],lines[1][1],lines[2][0]]):
    lines[0][2]=lines[1][1]=lines[2][0]='O'
    found = True


if found:
    for line in lines:
        print("".join(line))
else:
    print("false")