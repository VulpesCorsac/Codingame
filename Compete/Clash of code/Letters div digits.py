s=input()
n=sum(c.isdigit() for c in s)
a=sum(c.isalpha() for c in s)
print(int(round(a/n, 0)))