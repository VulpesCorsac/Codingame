def fill(board):
    for i in range(3):
        for j in range(3):
            if board[i][j] == '.' and \
               (board[i][(j+1)%3] == board[i][(j+2)%3] == 'O' or \
                board[(i+1)%3][j] == board[(i+2)%3][j] == 'O' or \
                (  i == j and board[(i+1)%3][(j+1)%3] == board[(i+2)%3][(j+2)%3] == 'O') or \
                (i+j == 2 and board[(i+1)%3][(j-1)%3] == board[(i+2)%3][(j-2)%3] == 'O')):
                board[i][j] = 'O'
                return True
    return False


board = [list(input()) for _ in range(3)]
if fill(board):
    print('\n'.join(''.join(L) for L in board))
else:
    print('false')
