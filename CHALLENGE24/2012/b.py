import sys
from scipy.spatial import KDTree as KDTree
blown = set()
points = {}

i = 0
for x, y, r in list(map(lambda x: tuple(map(lambda x: int(x), x.split())), open(sys.argv[1]).read().split("\n")[1:-1])):
	if not (x, y) in points or points[(x, y)][0] <= r:
		points[(x, y)] = (r, i)

	i += 1

#def dist(a, b):
#	return (a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2

result = []
tree = KDTree(list(points.keys()))
for x, y in sorted(list(points.keys()), key = lambda x: -points[x][0]):
	if (x, y) in blown:
		continue

	result.append(str(points[(x, y)][1]))
	blowing = []
	blowing.append((x, y))
	while len(blowing):
		x, y = blowing.pop()
		if (x, y) in blown:
			continue

		blown.add((x, y))
		#blowing.extend(filter(lambda z: not z in blown and dist(z, (x, y)) <= points[(x, y)][0] ** 2, list(points.keys())))
		blowing.extend(map(lambda x: tree.data[x], filter(lambda x: not x in blown, tree.query_ball_point((x, y), points[(x, y)][0]))))

print(' '.join(result))
