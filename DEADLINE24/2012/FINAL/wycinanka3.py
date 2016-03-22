import time
import sys
import hashlib
import random
import deadline24
from deadline24 import Server
from pygraph.classes.graph import graph as Graph
import pygraph.algorithms.sorting
import pygraph.algorithms.searching
topsort = pygraph.algorithms.sorting.topological_sorting
dfs = pygraph.algorithms.searching.depth_first_search
bfs = pygraph.algorithms.searching.breadth_first_search

class FoundSolution(Exception): pass
class TimeOut(Exception): pass

class Solver(object):
	def __init__(self, id):
		super(Solver, self).__init__()
		self._id = id
		self._d24 = Server(20003, 'SOLVER %d' % id)
		self._d24.connect()
		self._d24.login()
		self._d24.debug('Started!')
		self.graph = [list()]
		self.weight = [0]
		self.timeLeft = 0
		self.orders = 0
		self.order = None
		self.done = True
		self.lastOrder = 0
		self.time = 1

	def describeWorld(self):
		self.world = self._d24.command('DESCRIBE_WORLD')
		result = self.world.split('\n')
		self.world = hashlib.md5(self.world).hexdigest()
		res = result[0].split()
		self.verts, self.edges, self.time = map(int, res[:-1])
		self.scale = float(res[-1])
		self.graph = [list() for i in range(self.verts)]
		self.weight = [0] * self.verts
		self.lastOrder = 1
		for e in range(self.edges):
			start, end = map(lambda x: int(x) - 1, result[e + 1].split())
			self.graph[start].append(end)
			self.weight[start] += 1
			self.graph[end].append(start)
			self.weight[end] += 1

		return True

	def timeToCut(self):
		self.timeLeft = int(self._d24.command('TIME_TO_CUT'))
		return self.timeLeft

	def getOrderCount(self):
		self.orders = int(self._d24.command('GET_ORDER_COUNT'))
		return self.orders

	def describeOrder(self, id):
		try:
			result = self._d24.command('DESCRIBE_ORDER %d' % id).split('\n')

		except deadline24.CommandError, e:
			self._d24.debug(e)
			return False

		res = result[0].split()
		self.order = {}
		self.order['id'] = id
		self.order['verts'], self.order['edges'], self.order['cost'], self.order['teams'] = int(res[0]), int(res[1]), float(res[2]), int(res[3])
		self.order['graph'] = graph = [list() for i in range(self.verts)]
		self.order['pygraph'] = pgraph = Graph()
		for v in range(self.order['verts']):
			pgraph.add_node(v)

		self.order['weight'] = weight = [0] * self.order['verts']
		self.order['result'] = list(range(self.order['verts']))
		for e in range(self.order['edges']):
			start, end = map(lambda x: int(x) - 1, result[e + 1].split())
			pgraph.add_edge((start, end))
			graph[start].append(end)
			weight[start] += 1
			graph[end].append(start)
			weight[end] += 1

		return True

	def getScore(self):
		return int(self._d24.command('GET_SCORE'))

	def commitSolution(self):
		command = ('COMMIT_SOLUTION %d %d ' % (self.order['id'], self.order['verts'])) + ' '.join(map(lambda x: str(x + 1), self.order['result']))
		result = self._d24.command(command)
		if result != 'ACCEPTED':
			return False

		return True

	def run(self):
		i = 4
		while True:
			i += 1
			if i == 5:
				i = 0
				last = self.timeLeft
				if last < self.timeToCut():
					self.describeWorld()

			if self.lastOrder >= self.orders:
				self.getOrderCount()

			if self.done:
				try:
					input = open('/tmp/' + self.world + '1', 'r')
					self.lastOrder = int(input.read())
					input.close()

				except:
					self.lastOrder = 1

				if self.lastOrder <= self.orders:
					self.lastOrder += 1
				
				output = open('/tmp/' + self.world + '1', 'w')
				output.write(str(self.lastOrder))
				output.close()

				self.lastOrder -= 1
				self.describeOrder(self.lastOrder)
				self.done = False
			
			self._d24.debug('Rozwiazuje %d' % (self.lastOrder))
			if self.order['verts'] > 20:
				self.done = True
				continue

			timeout = 15
			#verts = list(range(self.order['verts']))
			try:
				self.commitSolution()

			except deadline24.CommandError, e:
				if int(str(e).split()[3]) == 102:
					self.done = True
					continue


			self.endit = time.time() + timeout
			try:
				try:
					verts = list(topsort(self.order['pygraph']))
					self.findSolution(list(verts))
					verts = list(reversed(verts))
					self.findSolution(list(verts))
					xxx = dfs(self.order['pygraph'])
					verts = xxx[1]
					self.findSolution(list(verts))
					verts = list(reversed(verts))
					self.findSolution(list(verts))
					verts = xxx[2]
					self.findSolution(list(verts))
					verts = list(reversed(verts))
					self.findSolution(list(verts))
					verts = bfs(self.order['pygraph'])[1]
					self.findSolution(list(verts))
					verts = list(reversed(verts))
					self.findSolution(list(verts))
					for i in range(20000):
						random.shuffle(verts)
						self.findSolution(list(verts))
						self.findSolution(list(reversed(verts)))

				except FoundSolution:
					self.done = self.commitSolution()

			except deadline24.CommandError, e:
				self._d24.debug(str(e))

			except TimeOut:
				pass

			self.done = True
			#raw_input('>>jol')

	def findSolution(self, todo, first = True, used = None, been = None):
		if self.endit < time.time():
			raise TimeOut()

		if todo == []:
			raise FoundSolution()

		if not used:
			used = [False] * self.verts

		if not been:
			been = [False] * self.order['verts']


		#print todo
		v = todo.pop()
		bylo = False
		neighbours = set()
		for n in self.order['graph'][v]:
			if been[n]:
				if not neighbours:
					neighbours = set(self.graph[self.order['result'][n]])

				else:
					neighbours = neighbours.intersection(set(self.graph[self.order['result'][n]]))

				bylo = True

		if not neighbours:
			if bylo or not first:
				return

			neighbours = range(self.verts)

		been[v] = True
		neighbours = list(filter(lambda x: self.weight[x] >= self.order['weight'][v], list(neighbours)))
		random.shuffle(neighbours)
		for n in neighbours:
			if used[n]:
				continue

			self.order['result'][v] = n
			used[n] = True
			self.findSolution(list(todo), False, list(used), list(been))
			used[n] = False

if __name__ == "__main__":
	game = Solver(int(sys.argv[1]))
	game.run()
