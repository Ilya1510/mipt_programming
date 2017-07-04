res = 0
for i in range(10):
    res += 4 / (2 * i + 1) * (1 - 2 * (i % 2))
print(res)
