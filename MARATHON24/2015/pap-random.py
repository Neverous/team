import random
import sys

height, width = map(int, sys.stdin.read().split())
data = list(range(1, width * height + 1))
random.shuffle(data)

for j in range(height):
    print(' '.join(map(str, data[:width])))
    data = data[width:]
