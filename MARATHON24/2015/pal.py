import sys

class Node:
    def __init__(self):
        self.left = None
        self.right = None
        self.id = None

data = sys.stdin.read().split('\n')
shapes = int(data[0])
data = data[1:]
trees = []

def read_tree(desc, p=0):
    node = Node()
    if desc[p] == 'C':
        return (node, p + 1, list([1]))

    node.left, p, stats_l = read_tree(desc, p + 1)
    node.right, p, stats_r = read_tree(desc, p)
    if len(stats_l) < len(stats_r) or (len(stats_l) == len(stats_r) and stats_l < stats_r):
        node.left, node.right = node.right, node.left

    stats = [1] + list(map(lambda p: p[0] + p[1], zip(stats_l, stats_r)))
    return (node, p, stats)

def move_trees(A, B, ids=2):
    if not A.left and not B.left:
        return ids

    if not A.left:
        print('G', A.id, ids, ids + 1)
        A.left = Node()
        A.left.id = ids
        A.right = Node()
        A.right.id = ids + 1
        ids += 2

        ids = move_trees(A.left, B.left, ids)
        ids = move_trees(A.right, B.right, ids)

    return ids

for s in range(shapes):
    line = data[0]
    data = data[1:]
    trees.append(read_tree(line)[0])

for tree in trees:
    start = Node()
    start.id = 1
    move_trees(start, tree)
    print('L 0')
