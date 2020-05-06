import sys


alphabet_size = ord('Z')-ord('A')+1+1
stones = 30


def getSymbolNum(c):
    if c == ' ':
        return 0
    else:
        return ord(c)-ord('A')+1


def getSymbolFromNum(i):
    if i == 0:
        return ' '
    return chr(ord('A')+i-1)


def getSymbolDistance(next, prev, operation):
    if (operation == '+'):
        return (getSymbolNum(next) - getSymbolNum(prev) + alphabet_size) % alphabet_size
    else:
        return (getSymbolNum(prev) - getSymbolNum(next) + alphabet_size) % alphabet_size


def getClosestSymbolInstructionPlain(c, prev=' '):
    if c == prev:
        return ''
    
    plus_len  = getSymbolDistance(c, prev, '+')
    minus_len = getSymbolDistance(c, prev, '-')
    if (plus_len <= minus_len):
        return '+'*plus_len
    else:
        return '-'*minus_len 


def getClosestSymbolInstructionFlush(c, prev=' '):
    if c == prev:
        return ''
    
    plus_len_plain  = getSymbolDistance(c, prev, '+')
    minus_len_plain = getSymbolDistance(c, prev, '-')
    if (plus_len_plain <= minus_len_plain):
        plain_min_len =  '+'*plus_len_plain
    else:
        plain_min_len =  '-'*minus_len_plain 
    
    plus_len_flush  = getSymbolDistance(c, ' ', '+')
    minus_len_flush = getSymbolDistance(c, ' ', '-')
    if (plus_len_flush <= minus_len_flush):
        flush_min_len =  '+'*plus_len_flush
    else:
        flush_min_len =  '-'*minus_len_flush
    
    if (prev != ' '):
        if (c != ' '):
            if (ord(c) <= ord('M')):
                flush_min_len = '[+]' + flush_min_len
            else:
                flush_min_len = '[-]' + flush_min_len
        else:
            if (ord(c) <= ord('M')):
                flush_min_len = '[+]'
            else:
                flush_min_len = '[-]'

    if len(plain_min_len) <= len(flush_min_len):
        return plain_min_len
    else:
        return flush_min_len


def getClosestSymbolInstruction(c, prev=' ', can_flush=False):
    if can_flush:
        return getClosestSymbolInstructionFlush(c, prev)
    else:
        return getClosestSymbolInstructionPlain(c, prev)


def getAnsSingleStone(magic_phrase):
    ans = ''
    prev = ' '
    for c in magic_phrase:
        ans += getClosestSymbolInstruction(c, prev, True) + '.'
        prev = c
    return ans


def getPositionDistance(next, prev, movement):
    distance = 0
    while next != prev:
        if movement == '>':
            prev += 1
        else:
            prev -= 1
        distance += 1
        prev %= stones
    return distance


def getPositionMoveInstriction(next, prev):
    if next == prev:
        return ''
    right_len = getPositionDistance(next, prev, '>')
    left_len = getPositionDistance(next, prev, '<')
    if (right_len <= left_len):
        return '>'*right_len
    else:
        return '<'*left_len 


def getAnsUsingBestStone(magic_phrase, base_on_stones=' ', can_flush=False):
    symbols_on_stones = dict((i, base_on_stones) for i in range(stones))
    bilbo_pos = 0
    ans = ''
    for c in list(magic_phrase):
#        print(c, file=sys.stderr)
        stone_ans = dict()
        for stone in range(stones):
            stone_ans[stone] = getPositionMoveInstriction(bilbo_pos, stone) +  getClosestSymbolInstruction(c, symbols_on_stones[stone])
        stone_ans_sorted = sorted(stone_ans.items(), key=lambda kv: len(kv[1]))
#        print(stone_ans_sorted, file=sys.stderr)
        best_stone = stone_ans_sorted[0][0]
#        print(best_stone, file=sys.stderr)
        symbols_on_stones[best_stone] = c
        bilbo_pos = best_stone
#        print(symbols_on_stones, file=sys.stderr)
        ans += stone_ans[best_stone] + '.'
    return ans


def getInvertedCommand(command):
    ans = ''
    for c in list(command):
        if c in ('+', '-'):
            if c == '+':
                ans += '-'
            else:
                ans += '+'
        else:
            ans += c
    return ans


def getBaseOnStones(c):
    setBase = getClosestSymbolInstruction(c, ' ')
    return getInvertedCommand(setBase) + '[>' + setBase + ']' + setBase


def getAnsUsingBestStoneWithPrecalculatedBase(magic_phrase, can_flush=False):
    min_ans = getAnsUsingBestStone(magic_phrase, ' ', can_flush)
    for i in range(ord('Z') - ord('A') + 1):
        base = chr(ord('A') + i)
        tmp_ans = getBaseOnStones(base) + getAnsUsingBestStone(magic_phrase, base, can_flush)
        if len(tmp_ans) < len(min_ans):
            min_ans = tmp_ans
    return min_ans


scheme = 256*[256*['']]
def fillScheme():
    for x in range(256):
        for y in range(256):
            d = y - x
            if d > 128:
                d -= 256
            if d < -128:
                d += 256
            
            if d > 0:
                scheme[x][y] = '+'*d
            else:
                scheme[x][y] = '-'*(-d)
    
    shortened = True
    while (shortened):
        shoterned = False

        for x in range(256):
            for n in range(1, 40):
                for d in range(1, 40):
                    j = x
                    y = 0
                    for i in range(256):
                        if j == 0:
                            break
                        j = (j - d + 256) & 255
                        y = (y + n) & 255
                    if j == 0:
                        tmp = "[" + '-'*d + '>' + "+"*n + "<]>"
                        if len(tmp) < len(scheme[x][y]):
                            scheme[x][y] = tmp
                            shoterned = True

                    j = x
                    y = 0
                    for i in range(256):
                        if j == 0:
                            break
                        j = (j + d) & 255
                        y = (y - n + 256) & 255
                    if j == 0:
                        tmp = '[' + '+'*d + '>' + '-'*n + '<]>'
                        if len(tmp) < len(scheme[x][y]):
                            scheme[x][y] = tmp
                            shoterned = True
        for i in range(256):
            for j in range(256):
                for k in range(256):
                    if len(scheme[x][y]) > len(scheme[x][z]) + len(scheme[z][y]):
                        scheme[x][y] = scheme[x][z] + scheme[z][y]
                        shoterned = True

#fillScheme()
#print(scheme, file=sys.stderr)
magic_phrase = list(input())
min_ans = getAnsSingleStone(magic_phrase)
#next_ans = getAnsUsingBestStone(magic_phrase)
#if len(next_ans) <= len(min_ans):
#    print('Best stone answer without flushing is better', file=sys.stderr)
#    min_ans = next_ans
next_ans = getAnsUsingBestStone(magic_phrase, ' ', True)
if len(next_ans) <= len(min_ans):
    print('Best stone answer with flushing is better', file=sys.stderr)
    min_ans = next_ans
#next_ans = getAnsUsingBestStoneWithPrecalculatedBase(magic_phrase)
#if len(next_ans) <= len(min_ans):
#    print('Best stone answer with precalculated base without flushing is better', file=sys.stderr)
#    min_ans = next_ans
next_ans = getAnsUsingBestStoneWithPrecalculatedBase(magic_phrase, True)
if len(next_ans) <= len(min_ans):
    print('Best stone answer with precalculated base with flushing is better', file=sys.stderr)
    min_ans = next_ans
print(min_ans)
