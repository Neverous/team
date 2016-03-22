# 2012
# Maciej Szeptuch
# II UWr
import sys
import time
from collections import deque
def RES(tab, val, start, end):
	for v in range(start, end + 1):
		if tab[v] != val:
			return False

	return True

def RES2(tab, val, start, end):
	for v in range(start, end + 1):
		tab[v] = val

class Train:
	def __init__(self, start, end, length, id):
		self.start = start
		self.end = end
		self.length = length
		self.parts = [127] * (length + 1)
		self.path = {}
		self.onroad = False
		self.timetable = []
		self.id = id
		self.out = False

	def finished(self):
		if self.parts[-1] != self.end:
#			print "DBG:", self.id, "not finished"
			return False

#		print "DBG:", self.id, "finished"
		return True

	def move(self, clock, reserved):
		print >> sys.stderr, "TRYING TO MOVE:", self.id, self.parts, ",", self.start, "->", self.end, "::", self.onroad, "::", self.path
		if not self.onroad:
			return

		if self.parts[0] != self.end and not RES(reserved[self.path[self.parts[0]]][self.path[self.parts[0]]], self.id, clock, clock + length):
			return

		RES2(reserved[self.path[self.parts[0]]][self.path[self.parts[0]]], self.id, clock, clock + length)
		if self.parts[0] != self.end:
			self.timetable.append((clock, self.path[self.parts[0]]))

		after = []
		for p, part in enumerate(self.parts):
			if p and self.parts[p - 1] != 127:
				after.append(self.path[part])

			elif p:
				after.append(part)

			else:
				after.append(self.path[part])

		#if self.parts[-1] != after[-1]:
		#	reserved[self.parts[-1]][self.parts[-1]] = 0
		#	reserved[self.parts[-1]][after[-1]] = 0
		#	reserved[after[-1]][self.parts[-1]] = 0

		print >>sys.stderr, "DBG: TRAIN", self.id, "MOVED:", self.parts, "->", after
		self.parts = after
		if after[-1] == self.end:
			self.onroad = False
			#reserved[after[-1]][after[-1]] = 0

	def countPath(self, verts, edge, reserved, clock, count = False):
		print >> sys.stderr, "SEARCHING FOR:", self.id, self.start, self.end
		que = deque([(0, self.start)])
		best = [(1048576, -1)] * verts
		best[self.start] = (0, -1)
		self.path = {}
		while que:
			w, v = que.popleft()
			if not RES(reserved[v][v], 0, clock + w, clock + w + length):
				continue

			#print "DBG:", v, w
			if w > best[v][0]:
				continue

			for n, e in enumerate(edge[v]):
				#print "SCIEZKA:", v, n, e
				if not e or n == v or not RES(reserved[v][n], 0, clock + w, clock + w + length) or not RES(reserved[n][v], 0, clock + w, clock + w + length) or not RES(reserved[n][n], 0, clock + w, clock + w + length) or best[n][0] <= w + 1:
					continue

				best[n] = (w + 1, v)
				#print "DBG: QUE +=", (w + 1, n)
				#print "DBG: BEST =", best[n]
				que.append((w + 1, n))

		if best[self.end][0] != 1048576:
			act = self.end
			while best[act][1] != -1:
				self.path[best[act][1]] = act
				act = best[act][1]

			self.path[127] = self.start
			self.path[self.end] = self.end

		if self.path:
			print >>sys.stderr, "DEBUG:", self.id, "FOUND PATH:", self.path
			if count:
				return len(self.path)

			act = self.end
			tm = clock + self.length + best[self.end][0]
			while act != self.start:
				RES2(reserved[act][best[act][1]], self.id, tm - self.length, tm)
				RES2(reserved[best[act][1]][act], self.id, tm - self.length, tm)
				RES2(reserved[act][act], self.id, tm - self.length, tm)
				RES2(reserved[best[act][1]][best[act][1]], self.id, tm - self.length, tm)
				tm -= 1
				act = best[act][1]

			#for s, e in self.path.items():
			#	if s != 127:
					#print "RESERVING:", s, e, self.id
			
			#reserved[s][s] = self.id
			##		reserved[s][e] = self.id
			#		reserved[e][s] = self.id
			#		reserved[e][e] = self.id

		return 1048576

# INPUT
data = sys.stdin.read().split("\n")
verts, edges, trains = map(int, data[0].split())
edge = [[False] * 128 for v in range(128)]
p = 1
available = []
for e in range(edges):
	start, end = map(int, data[p].split())
	start -= 1
	end -= 1
	edge[start][end] = True
	edge[end][start] = True
	p += 1

for t in range(trains):
	start, end, length = map(int, data[p].split())
	start -= 1
	end -= 1
	available.append(Train(start, end, length, t + 1))
	p += 1

# LOGIC
work = True
clock = 0
reserved = [[[0] * 2048 for _ in range(128)] for _ in range(128)]
moving = []
while work:
	#print "STATE:", reserved
	toRemove = []
	for train in moving:
		if not train.onroad:
			toRemove.append(train)

	for train in toRemove:
		moving.remove(train)

	for train in moving:
		train.move(clock, reserved)

	work = False
	for train in available:
		work |= not train.finished()

	if not work:
		break

	for train in available:
		if not train.onroad and not train.finished():
			train.countPath(verts, edge, reserved, clock)
			if train.path:
				train.onroad = True
				moving.append(train)

	clock += 1
	#time.sleep(2)

# RESULT
for train in available:
	print len(train.timetable)
	for time, pos in train.timetable:
		print time, pos + 1
