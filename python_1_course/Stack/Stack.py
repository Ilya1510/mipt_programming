import sys


class Stack:
    def __init__(self, x):
        self.values = []
        for i in x:
            self.values.append(i)

    def __len__(self):
        return len(self.values)

    def __str__(self):
        res = ""
        for i in self.values:
            res += str(i) + ' '
        return res.strip(' ')

    def push(self, x):
        self.values.append(x)

    def pop(self):
        self.values.pop()

    def top(self):
        return self.values[-1]


exec(sys.stdin.read())
