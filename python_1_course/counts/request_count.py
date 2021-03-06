import sys

count = [["Windows", 0], ["Ubuntu", 0], ["OS X", 0], ["Unknown", 0]]
for line in sys.stdin:
    parts = line.split('"')
    while (parts[-1] == '\n' or parts[-1] == ''):
        parts.pop()
    user_agent = parts[-1]
    if (user_agent.find("Windows") != -1):
        count[0][1] += 1
    elif (user_agent.find("Ubuntu") != -1):
        count[1][1] += 1
    elif (user_agent.find("Macintosh") != -1):
        count[2][1] += 1
    else:
        count[3][1] += 1

for ans in sorted(count, key=lambda x: x[1]):
    print(ans[0] + ": " + str(ans[1]))
