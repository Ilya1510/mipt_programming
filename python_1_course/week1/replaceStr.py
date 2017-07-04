line1 = ""
line2 = ""
for i in range(1, 101):
    if (i % 15 == 0):
        line1 += "BazQux"
    elif (i % 5 == 0):
        line1 += "Qux"
    elif (i % 3 == 0):
        line1 += "Baz"
    else:
        line1 += str(i)
    line2 += str(i)
    line1 += ' '
    line2 += ' '
line1 = line1.strip()
line2 = line2.strip()
print(line2)
print(line1)
