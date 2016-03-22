# -*- encoding: utf-8 -*-

from collections import defaultdict
from zuk import Zuk
from town import Town
from person import Person
from parser import Parser
from road import Road
import time
import random

P = Parser()

class Game(object):
	def __init__(self, server, debug, game = 1):
		self.server = server
		self.dbg = debug
		self.cash = 0
		self.write = server.write
		self.time = 0
		self.zuki = {}
		self.roads = {}
		self.towns = {}
		self.towns_count = 0
		self.people = {}
		self.connections = defaultdict(lambda: defaultdict(lambda: []))
		self.parsed = defaultdict(lambda: defaultdict(lambda: False))
		self.distance = defaultdict(lambda: 1000000000)
		self.isMine = defaultdict(lambda: False)
		self.game = game
		self.actZuk = None
		self.parse()

	def parse(self):
		data = open(u'graph%d' % self.game, u'r').read()
		for line in data.split(u'\n'):
			line = line.strip()
			if not line:
				continue
			line = line.split(u' ')

			self.parsed[int(line[0])][int(line[1])] = True
			self.parsed[int(line[1])][int(line[0])] = True
			for road in line[2:]:
				try:
					self.connections[int(line[1])][int(line[0])].append(int(road))
					self.connections[int(line[0])][int(line[1])].append(int(road))

				except:
					pass

		data = open(u'roads%d' % self.game, u'r').read()
		for line in data.split(u'\n'):
			line = line.strip()
			if not line:
				continue
			line = line.split(u' ')

			self.roads[int(line[0])] = Road(
				int(line[0]),
				int(line[1]),
				int(line[2]),
				float(line[3]),
				float(line[4]),
				float(line[5]),
			)

	def save(self):
		plik = open(u'graph%d' % self.game, u'w')
		edges = []
		for start in self.parsed:
			for end in self.parsed[start]:
				edges.append((start, end))

		for edge in edges:
			print >>plik, edge[0], edge[1],
			for road in self.connections[edge[0]][edge[1]]:
				print >>plik, road,

			print >>plik, u''

		plik.close()

		plik = open(u'roads%d' % self.game, u'w')

		for road in self.roads.values():
			print >>plik, '%d %d %d %.2f %.2f %.2f' % (
				road.getId(),
				road.getStart(),
				road.getEnd(),
				road.getSpeed(),
				road.getCost(),
				road.getLength(),
				)

		plik.close()

	def read(self):
		data = self.server.read()
		if data.split()[0] <> u'OK':
			raise IOError(u'Communication failed: %s' % data)

		return u'\n'.join(data.split(u'\n')[1:])

	def commandME(self):
		self.write(u'ME')
		data = self.read()
		zuk = P.parseZUK(data)
		zuk.setTimeout(self.commandTIME() + zuk.getTimeout())
		if zuk.getId() in self.zuki:
			zuk.visited = list(self.zuki[zuk.getId()].visited)

		self.actZuk = self.zuki[zuk.getId()] = zuk
		return zuk

	def commandCASH(self):
		self.write(u'CASH')
		data = self.read().split()
		self.cash = float(data[0])
		return self.cash

	def commandCARS(self):
		self.write(u'CARS')
		data = self.read().split(u'\n')
		count = int(data[0])
		data = data[1:]
		for c in xrange(0, 2 * count, 2):
			zuk = P.parseZUKInfo(u'\n'.join(data[c:c + 2]))
			if zuk.getId() in self.zuki:
				zuk.visited = list(self.zuki[zuk.getId()].visited)
			self.zuki[zuk.getId()] = zuk

		return True

	def commandTAKE(self, personId):
		self.write(u'TAKE %d' % personId)
		try:
			result = self.read()
			self.commandME()

		except:
			pass

		del self.people[personId]
		return True

	def commandTIME(self):
		self.write(u'TIME')
		time = self.read().split(u' ')
		self.time = float(time[0])
		return self.time

	def commandSWITCH(self, zukId):
		self.write(u'SWITCH %d' % zukId)
		result = self.read()
		self.actZuk = self.zuki[zukId]
		self.commandME()
		return self.actZuk

	def commandTOWNS(self):
		self.write(u'TOWNS')
		data = self.read().split(u'\n')
		self.towns_count = count = int(data[0])
		data = data[1:]
		for c in xrange(count):
			town = P.parseTOWN(data[c])
			self.towns[town.getId()] = town

		return True

	def commandWAYS(self):
		self.write(u'WAYS')
		data = self.read().split(u'\n')
		count = int(data[0])
		data = data[1:]
		for c in xrange(count):
			road = P.parseROAD(data[c])
			road.setStart(self.actZuk.getPlace())
			self.roads[road.getId()] = road
			if not road.getId() in self.connections[road.getStart()][road.getEnd()]:
				self.connections[road.getStart()][road.getEnd()].append(road.getId())

			if not road.getId() in self.connections[road.getEnd()][road.getStart()]:
				self.connections[road.getEnd()][road.getStart()].append(road.getId())

			self.parsed[road.getStart()][road.getEnd()] = True
			self.parsed[road.getEnd()][road.getStart()] = True

		return True

	def commandGO(self, roadId):
		self.write(u'GO %d' % roadId)
		response = self.read()
		self.commandME()
		return True

	def commandLIST(self):
		self.write(u'LIST')
		data = self.read().split(u'\n')
		count = int(data[0])
		data = data[1:]
		self.people = {}
		for c in xrange(count):
			person = P.parsePERSON(data[c])
			self.people[person.getId()] = person

		return True

	def commandMYROADS(self):
		self.write(u'MY_ROADS')
		data = self.read().split(u'\n')
		count = int(data[0])
		data = data[1:]

		for c in xrange(count):
			self.isMine[int(data[c].split()[0])] = True
			try:
				self.roads[int(data[c].split()[0])].setCost(0)

			except:
				pass

	def commandWAIT(self):
		self.write(u'WAIT')
		data = self.read()
		if data.split()[-1] == u'OK':
			return True

		data = self.read()
		return True

	def getBestSpeed(self, a, b):
		if not self.connections[a][b]:
			return -1

		best = self.connections[a][b][0]
		for road in self.connections[a][b]:
			if not road in self.roads or self.roads[road].getCost() > 100:
				continue

			if self.roads[road].getLength() / self.roads[road].getSpeed() < self.roads[best].getLength() / self.roads[best].getSpeed():
				best = road

		if self.roads[best].getCost() > 100:
			return -1

		return best

	def recalculateCosts(self):
		self.cost = defaultdict(lambda: defaultdict(lambda: 1000000000))
		for s in xrange(self.towns_count):
			for e in xrange(self.towns_count):
				pos = self.getBestSpeed(s, e)
				if pos == -1:
					continue

				self.cost[s][e] = self.roads[pos].getLength() / self.roads[pos].getSpeed()

		for s in xrange(self.towns_count):
			for e in xrange(self.towns_count):
				for t in xrange(self.towns_count):
					self.cost[s][e] = min(self.cost[s][e], self.cost[s][t] + self.cost[t][e])

		#for s in xrange(self.towns_count):
			#for e in xrange(self.towns_count):
				#print u'%d -> %d: %.2f' % (s, e, self.cost[s][e])

	def findBest(self, start, endpoints):
		if not endpoints:
			return []

		endpoint = endpoints[0]
		for possible in endpoints:
			if self.cost[start][possible] < self.cost[start][endpoint]:
				endpoint = possible

		if self.cost[start][endpoint] == 1000000000:
			return []

		return self.shortestPath(start, endpoint)

	def shortestPath(self, start, end):
		class Pair(list):
			def __hash__(self):
				return self[0]

		distance = defaultdict(lambda: 1000000000)
		distance[start] = 0
		by = {}
		que = set()
		que.add(Pair([0, start]))
		while que:
			cost, act = que.pop()
			if act == end and cost == self.cost[start][end]:
				result = []
				while act <> start:
					result = [by[act][1], ] + result
					act = by[act][0]

				return result

			for neighbour in self.connections[act].keys():
				pos = self.getBestSpeed(act, neighbour)
				if pos == -1:
					continue

				if distance[neighbour] < distance[act] + self.cost[act][neighbour]:
					continue

				distance[neighbour] = distance[act] + self.cost[act][neighbour]
				que.add(Pair([distance[neighbour], neighbour]))
				by[neighbour] = [act, pos]

		return []

	def run(self, autko):
		self.commandTIME()
		self.commandCASH()
		self.commandCARS()
		self.commandTOWNS()
		self.commandSWITCH(autko)
		self.commandME()
		route = []
		while True:
			self.commandMYROADS()
			self.commandWAIT()
			self.commandWAYS()
			self.save()
			self.recalculateCosts()

			if not route and self.actZuk.getFree():
				self.commandLIST()

			if self.actZuk.purgePeople():
				print u'ZUCZEK U CELU!!!!'

			if self.actZuk.getFree():
				for person in self.people.values():
					if person.getSource() == self.actZuk.getPlace():
						self.commandTAKE(person.getId())

			if not route and self.actZuk.getFree():
				route = self.findBest(self.actZuk.getPlace(), [person.getSource() for person in self.people.values()])

			if not route and self.actZuk.getUsed():
				route = self.findBest(self.actZuk.getPlace(), [person.getDestination() for person in self.actZuk.people.values()])

			if not route:
				route = self.findBest(self.actZuk.getPlace(), self.connections[self.actZuk.getPlace()].keys())

			if not route:
				raise IOError(u'DUPA!')

			print self.actZuk
			print route
			self.commandGO(route[0])
			del route[0]
