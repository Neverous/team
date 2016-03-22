import sys
from shapes import SHAPES
from copy import deepcopy

data = sys.stdin.read().split()
tests = int(data[0])
data = data[1:]

def available(field, shape, H, W, house=True):
    gain = 0
    for h, row in enumerate(shape):
        for w, cell in enumerate(row):
            if H + h < 0 or W + w < 0:
                return None

            if H + h >= len(field) or W + w >= len(field[H + h]):
                return None

            if cell and field[H + h][W + w] in ('#', 'H', 'G'):
                return None

            if cell and field[H + h][W + w] == 'X':
                gain += (house and -1 or 1) * 2

            if house:
                if cell and H + h - 1 >= 0 and field[H + h - 1][W + w] == 'H':
                    gain -= 1

                if cell and H + h + 1 < len(field) and field[H + h + 1][W + w] == 'H':
                    gain -= 1

                if cell and W + w - 1 >= 0 and field[H + h][W + w - 1] == 'H':
                    gain -= 1

                if cell and W + w + 1 < len(field[H + h]) and field[H + h][W + w + 1] == 'H':
                    gain -= 1

    return gain

def fill(field, shape, H, W, what='H'):
    result = deepcopy(field)
    for h, row in enumerate(shape):
        for w, cell in enumerate(row):
            if cell:
                result[H + h][W + w] = what

    return result

def best_match(field, house=None):
    if not house:
        current = (0, [])
        for h, row in enumerate(field):
            for w, cell in enumerate(row):
                for s, r, mark, shape in SHAPES:
                    gain = available(field, shape, h, w)
                    if gain is not None:
                        #print('Put house', gain, s, r, h, w)
                        cost, garden, match = best_match(fill(field, shape, h, w), (h, w, shape))
                        if garden is not None and cost + gain > current[0]:
                            current = (cost + gain, [((s, r, (1 + mark[0] + w, 1 + mark[1] + h)), garden), ] + match)

        return current

    current = (0, None, [])
    H, W, house = house
    for h, row in enumerate(house):
        for w, cell in enumerate(row):
            if cell > 0:
                if h-1 < 0 or house[h-1][w] == ' ':
                    for s, r, mark, shape in SHAPES:
                        for o in range(0, len(shape[0])):
                            if shape[-1][o] == 0: continue
                            gain = available(field, shape, H + h - len(shape), W + w - o, False)
                            if gain is not None:
                                gain += 3
                                #print('Put garden', gain, s, r, H + h - len(shape), W + w - o)
                                #print('\n'.join([''.join(row) for row in fill(field, shape, H + h - len(shape), W + w - o, 'G')]))
                                return (gain, (s, r, (1 + mark[1] + W + w - o, 1 + mark[0] + H + h - len(shape))), [])
                                cost, match = best_match(fill(field, shape, H + h - len(shape), W + w - o, 'G'))
                                if cost + gain > current[0]:
                                    current = (cost + gain, (s, r, (1 + mark[1] + W + w - o, 1 + mark[0] + H + h - len(shape))), match)

                if h + 1 >= len(house) or house[h+1][w] == ' ':
                    for s, r, mark, shape in SHAPES:
                        for o in range(0, len(shape[0])):
                            if shape[0][o] == 0: continue
                            gain = available(field, shape, H + h + 1, W + w - o, False)
                            if gain is not None:
                                gain += 3
                                #print('Put garden', gain, s, r, H + h + 1, W + w - o)
                                #print('\n'.join([''.join(row) for row in fill(field, shape, H + h + 1, W + w - o, 'G')]))
                                return (gain, (s, r, (1 + mark[1] + W + w - o, 1 + mark[0] + H + h + 1)), [])
                                cost, match = best_match(fill(field, shape, H + h + 1, W + w - o, 'G'))
                                if cost + gain > current[0]:
                                    current = (cost + gain, (s, r, (1 + mark[1] + W + w - o, 1 + mark[0] + H + h + 1)), match)

    return current

for t in range(tests):
    height, width = map(int, data[:2])
    field = list(map(list, data[2:2+height]))
    cost, match = best_match(field)
    print(len(match), cost)
    for house, garden in match:
        shape, rotation, mark = house
        print(shape, rotation, ' '.join(map(str, mark)), end=' ')
        shape, rotation, mark = garden
        print(shape, rotation, ' '.join(map(str, mark)))

    """
    after = field
    for house, garden in match:
        for s, r, mark, shape in SHAPES:
            if s != house[0]: continue
            if r != house[1]: continue

            pos = (house[2][0] - mark[0] - 1, house[2][1] - mark[1] - 1)
            after = fill(after, shape, pos[1], pos[0])
            break

        for s, r, mark, shape in SHAPES:
            if s != garden[0]: continue
            if r != garden[1]: continue

            pos = (garden[2][0] - mark[0] - 1, garden[2][1] - mark[1] - 1)
            after = fill(after, shape, pos[1], pos[0], 'G')
            break

    print('\n'.join([''.join(row) for row in after]))
    print("")
    print('\n'.join([''.join(row) for row in field]))
    """
    data = data[2+height:]
