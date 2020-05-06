db = dict()
n = int(input())  # Number of elements which make up the association table.
q = int(input())  # Number Q of file names to be analyzed.
for i in range(n):
    # ext: file extension
    # mt: MIME type.
    ext, mt = input().split()
    db[ext.lower()] = mt
    
    
for i in range(q):
    file = input()
    
    if "." in file:
        ext = file.split(".")[-1].lower()
        if ext in db.keys():
            print(db[ext])
        else:
            print("UNKNOWN")
    else:
        print("UNKNOWN")