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
		try:
			self.parse()

		except:
			pass

	def parse(self):
		data = open(u'graph%d' % self.game, u'r').read()
		for line in data.split(u'\n'):
			line = line.strip()
			if not line:
				continue
			line = line.split(u' ')

			self.parsed[int(line[0])][int(line[1])] = True
			self.parsed[int(line[1])][int(line[0])] = True
			try:
				self.connections[int(line[1])][int(line[0])].append(int(line[2]))
				self.connections[int(line[0])][int(line[1])].append(int(line[2]))

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
			print >>plik, edge[0], edge[1]

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

	def countVertices(self):
		start = self.actZuk.getPlace()
		for c in xrange(self.towns_count):
			self.distance[c] = 1000000000

		self.distance[start] = 0

		queue = []
		queue.append(start)
		while queue:
			act = queue[0]
			del queue[0]

			for neighbour in self.parsed[act].keys():
				if self.distance[neighbour] < self.distance[act] + 1:
					continue

				self.distance[neighbour] = self.distance[act] + 1
				queue.append(neighbour)

		return

	def getShortestPath(self, start, end):
		by = {}
		result = []
		distance = {}
		for c in xrange(self.towns_count):
			distance[c] = 1000000000

		distance[start] = 0
		by[start] = start

		queue = []
		queue.append(start)
		while queue:
			act = queue[0]
			del queue[0]
			if act == end:
				while act <> start:
					result.append(act)
					act = by[act]

				return list(reversed(result))

			for neighbour in self.connections[act].keys():
				if distance[neighbour] < distance[act] + 1:
					continue

				distance[neighbour] = distance[act] + 1
				queue.append(neighbour)
				by[neighbour] = act

		return -1

	def getEdgesFrom(self, vert, remove = []):
		result = []
		for end, edges in self.connections[vert].items():
			result += edges

		return self.gimmeh(result, remove)

	def gimmeh(self, a, b):
		return [x for x in a if not x in b]

	def sortEdges(self, a, b):
		if self.isMine[a]:
			return -1

		if self.isMine[b]:
			return 1

		if self.roads[a].getCost() > 100:
			return 1

		x = self.roads[a].getLength() / self.roads[a].getSpeed() - self.roads[b].getLength() / self.roads[b].getSpeed()
		if x > 0:
			return 1

		if x < 0:
			return -1

		return 0

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

			if self.actZuk.purgePeople():
				print 'KUUUUUUUUUUUUUURRRRRRRRRRRRRRRRWWWWWWWWWWWWWWWWWWWWWWWAAAAAAAAAAAAAAAAAAAAAAaa!!!!!!!!!'

			if not route and self.actZuk.getUsed():
					for person in self.actZuk.people.values():
						route = self.getShortestPath(self.actZuk.getPlace(), person.getDestination())
						if route:
							break

					if not route or route == -1:
						something = sorted(self.getEdgesFrom(self.actZuk.getPlace(), self.actZuk.visited), self.sortEdges)
						route = [self.roads[something[0]].getEnd(), ]

			if not route:
				if self.actZuk.getFree():
					self.commandLIST()
					for person in self.people.values():
						if person.getSource() == self.actZuk.getPlace():
							self.commandTAKE(person.getId())

				if self.actZuk.getFree():
					self.countVertices()
					best = self.people.values()[0]
					for person in self.people.values():
						if self.distance[person.getSource()] < self.distance[best.getSource()]:
							best = person

					route = self.getShortestPath(self.actZuk.getPlace(), best.getSource())
					print 'R2', route
					if not route or route == -1:
						something = sorted(self.getEdgesFrom(self.actZuk.getPlace(), self.actZuk.visited), self.sortEdges)
						#print 'POSORTOWANE:'
						#for sth in something:
							#print self.roads[sth]

						route = [self.roads[something[0]].getEnd(), ]

				else:
					self.countVertices()
					for person in self.actZuk.people.values():
						route = self.getShortestPath(self.actZuk.getPlace(), person.getDestination())
						if route:
							break
					print 'R1', route
					if not route or route == -1:
						something = sorted(self.getEdgesFrom(self.actZuk.getPlace(), self.actZuk.visited), self.sortEdges)
						#print 'POSORTOWANE2:'
						#for sth in something:
							#print self.roads[sth]

						route = [self.roads[something[0]].getEnd(), ]

				if self.actZuk.getFree():
					for person in self.people.values():
						if person.getSource() == self.actZuk.getPlace():
							self.commandTAKE(person.getId())

			if route == -1:
				something = sorted(self.getEdgesFrom(self.actZuk.getPlace(), self.actZuk.visited), self.sortEdges)
				route = [self.roads[something[0]].getEnd(), ]

			print 'JEDZIEMY:', route
			print route[0]
			print self.actZuk

			sth = sorted(self.connections[self.actZuk.getPlace()][route[0]], self.sortEdges)
			print u'STH:', sth
			#print 'BEST:'
			#for sth in best:
				#print self.roads[sth]

			best = 0
			temp = 0
			try:
				while best < len(sth) and ((not self.isMine[sth[best]] and self.roads[sth[best]].getCost() > 100) or sth[best] in self.actZuk.visited):
					best += 1

				if best == len(sth):
					best -= 1

				#if not self.isMine[sth[best]] and self.roads[sth[best]].getCost() > 300:
					#raise

			except:
				data = raw_input(u'>>')
				best = temp
				if data.lower() <> 'y':
					raise AttributeError('OOOOOO FUCK!!!!')

			print best
			self.commandGO(sth[best])
			self.commandWAIT()
			time.sleep(1)
			self.actZuk.visited.append(sth[best])
			del route[0]

