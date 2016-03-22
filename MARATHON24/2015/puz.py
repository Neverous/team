import random
import sys

# INPUT
data = sys.stdin.read().strip().split('\n')
width, height, colors = map(int, data[0].split())

setup = [[None for _ in range(width)] for _ in range(height)]
sides = [[set() for _ in range(colors + 1)] for _ in range(4)]
puzzles = []
used = set()

data = data[1:]
for puzzle in range(width * height):
    line = data[0]
    data = data[1:]
    k1, k2, k3, k4 = map(int, line.split())
    puzzles.append((k1, k2, k3, k4))
    sides[0][k1].add(puzzle)
    sides[1][k2].add(puzzle)
    sides[2][k3].add(puzzle)
    sides[3][k4].add(puzzle)

valid_pieces = int(data[0])
data = data[1:]
for _ in range(valid_pieces):
    line = data[0]
    data = data[1:]
    x, y, num, o = map(int, line.split())
    used.add(num)

    setup[y][x] = (num, o)

# SOLUTION
def get_color(y, x, side):
    if not 0 <= y < height or not 0 <= x < width:
        return 0

    if setup[y][x] is None:
        return None

    ret = puzzles[setup[y][x][0]][(4 + side - setup[y][x][1]) % 4]
    return ret

def choose_puzzle(*args):
    valid = set(filter(lambda x: x not in used, range(width * height)))
    for rot in range(4):
        possible = set(valid)
        for i in range(4):
            if args[i] is not None:
                possible &= sides[(4 - rot + i) % 4][args[i]]

        if possible:
            num = random.choice(list(possible))
            used.add(num)
            return (num, rot)

    return None

positions = [(y, x) for y in range(height) for x in range(width)]
positions = [(0, x) for x in range(width)] + [(height - 1, x) for x in range(width)] + [(y, 0) for y in range(height)] + [(y, width - 1) for y in range(height)] + positions
random.shuffle(positions)
for y, x in positions:
    if setup[y][x] is not None:
        continue

    left    = get_color(y, x - 1, 2)
    top     = get_color(y - 1, x, 3)
    right   = get_color(y, x + 1, 0)
    bottom  = get_color(y + 1, x, 1)

    setup[y][x] = choose_puzzle(left, top, right, bottom)

# OUTPUT
result = 0
y = 0
for row in setup:
    x = 0
    for piece in row:
        if piece is None:
            print('-1 -1', end=' ')

        else:
            result += 1
            result += 3 * (get_color(y, x - 1, 2) == get_color(y, x, 0))
            result += 3 * (get_color(y - 1, x, 3) == get_color(y, x, 1))
            result += 3 * (get_color(y, x + 1, 0) == get_color(y, x, 2))
            result += 3 * (get_color(y + 1, x, 1) == get_color(y, x, 3))
            print(*piece, end=' ')

        x += 1

    y += 1
    print('')

print(result, file=sys.stderr)
