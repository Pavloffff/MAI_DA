import random
n = int(input())
f = open('test.txt', 'w')

f.write("%d %d\n" %( n, n))

s = ""

for i in range(n):
    for j in range(n):
        s += "%d" % (random.randint(1, 51))
        s += " "
    s += "\n"

f.write(s)
f.close()