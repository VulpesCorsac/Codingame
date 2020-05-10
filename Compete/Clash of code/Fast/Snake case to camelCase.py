p = input()
A = p.split('_')
for i in range(1, len(A)):
    A[i] = A[i].title()
print(''.join(A))