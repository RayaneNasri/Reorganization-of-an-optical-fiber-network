l = []
for x in range(11):
    for y in range(11):

        n = y + (x + y)*(x + y + 1) // 2
        if n in l:
            print("Collision !")
        l.append(n)
        