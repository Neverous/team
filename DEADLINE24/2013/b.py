from collections import defaultdict
from multiprocessing import Process, Queue, Value
import sys
import copy
WORKERS = 1
color_map = {
    0: '.',
    1: '#',
}
visited = defaultdict(lambda: 0)

workers = []
que = Queue()
result = Queue()
stop = Value('b', False)

for c, b in enumerate(range(ord('A'), ord('Z') + 1)):
    color_map[c + 2] = chr(b)

for c, b in enumerate(range(ord('a'), ord('z') + 1)):
    color_map[c + 28] = chr(b)

def niceShape(shape):
    return '\n'.join([''.join(map(lambda x: color_map[x], row)) for row in shape])

def dfs(field, (x, y), size, path = []):
    print >>sys.stderr, 'DFS', (x, y), size, path
    if not size:
        yield path

    visited[(x, y)] = True
    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        print >>sys.stderr, 'Sprawdzam:', (dx, dy), (x + dx, y + dy)
        if 0 <= x + dx < len(field[0]) and 0 <= y + dy < len(field) and not visited[(x + dx, y + dy)] and field[y + dy][x + dx] == 1:
            print >>sys.stderr, 'Wchodze'
            visited[(x + dx, y + dy)] = True
            path.append((x + dx, y + dy))
            for _path in dfs(field, (x + dx, y + dy), size - 1, path):
                yield _path

            path.pop()
            visited[(x + dx, y + dy)] = False

def generateShapes(field, size):
    global visited
    visited = defaultdict(lambda: False)
    for y, row in enumerate(field):
        for x, cell in enumerate(row):
            if cell == 1:
                print >>sys.stderr, 'Generowanie sciezek', (x, y), size
                for path in dfs(field, (x, y), size - 1, [(x, y)]):
                    sx, sy = min([x for x, _ in path]), min([y for _, y in path])
                    ex, ey = max([x for x, _ in path]), max([y for _, y in path])
                    w = ex - sx + 1
                    h = ey - sy + 1
                    shape = [[0 for _ in range(w)] for _ in range(h)]
                    for x, y in path:
                        shape[y - sy][x - sx] = 1

                    print >>sys.stderr, 'Ksztalt:', shape
                    yield shape

def rotations(shape):
    for r in range(4):
        yield shape
        shape = zip(*shape[::-1])

def match(field, x, y, shape):
    for r, row in enumerate(shape):
        for c, cell in enumerate(row):
            if cell != 1: continue
            if field[y + r][x + c] != 1:
                return False

    return True

def paste(field, x, y, shape, color):
    for r, row in enumerate(shape):
        for c, cell in enumerate(row):
            if cell != 1: continue
            field[y + r][x + c] = color

def check(field):
    for row in field:
        for cell in row:
            if cell == 1:
                return False

    return True

def startWorkers():
    global que
    global result
    que = Queue()
    result = Queue()
    stop.value = False
    def worker(_id, stop, que, res):
        while not stop.value:
            fld, h, w, shape, color = que.get()
            print >>sys.stderr, _id, 'Przetwarzanie\n' + niceShape(fld), h, w, shape, color
            if color < 2 or h >= len(fld): # koniec
                print >>sys.stderr, 'SPRAWDZAM', color
                if color < 2 and check(fld): # pasuje
                    print >>sys.stderr, 'MAMY WYNIK1!!!'
                    res.put(fld)

                continue

            if w >= len(fld[h]): # poza granicami
                w = 0
                h += 1
                que.put((fld, h, w, shape, color))
                continue

            if fld[h][w] != 1: # juz pokolorowane
                w += 1
                que.put((fld, h, w, shape, color))
                continue

            for _shape in rotations(shape): # dla kazdej rotacji
                print >>sys.stderr, 'Rotacja:\n' + niceShape(_shape)
                for y in range(max(h - len(_shape) + 1, 0), min(h, len(fld) - len(_shape)) + 1):
                    for x in range(max(w - len(_shape[0]) + 1, 0), min(w, len(fld[0]) - len(_shape[0])) + 1): # dla kazdego mozliwego przesuniecia
                        print >>sys.stderr, _id, 'Przypasowywanie\n' + niceShape(_shape) + '\ndo\n' + niceShape(fld) + '\n w', (x, y)
                        if match(fld, x, y, _shape): # przypasuj
                            _fld = copy.deepcopy(fld)
                            paste(_fld, x, y, _shape, color) # pokoloruj
                            print >>sys.stderr, _id, 'Pasuje!\n' + niceShape(_fld)
                            que.put((_fld, h, w + 1, shape, color - 1))

    for w in range(WORKERS):
        p = Process(target = worker, args=(w, stop, que, result))
        p.start()
        workers.append(p)

def stopWorkers():
    global workers
    stop.value = True
    for worker in workers:
        que.put(None)

    for worker in workers:
        worker.terminate()
        worker.join()

    workers = []

if __name__ == '__main__':
    try:
        tests = int(raw_input())
        for t in range(tests):
            startWorkers()
            field = []
            shapes = []
            size = 0
            colors = int(raw_input())
            height, width = map(int, raw_input().split())
            for _ in range(height):
                field.append(list(map(lambda x: x == '#' and 1 or 0, raw_input())))

            for row in field:
                for cell in row:
                    size += cell

            size /= colors
            for shape in generateShapes(field, size):
                que.put((field, 0, 0, shape, colors + 1))

            print niceShape(result.get())
            stopWorkers()
    except:
        for worker in workers:
            worker.terminate()
