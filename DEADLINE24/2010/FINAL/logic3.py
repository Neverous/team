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

	def commandWAIT(self):
		self.write(u'WAIT')
		data = self.read()
		if data.split()[-1] == u'OK':
			return True

		data = self.read()
		return True

	def getRoads(self, data):
		edges = []
		for end in data.values():
			edges += end

		return list(set(edges))

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

			self.commandME()
			if self.actZuk.purgePeople():
				print 'KUUUUUUUUUUUUUURRRRRRRRRRRRRRRRWWWWWWWWWWWWWWWWWWWWWWWAAAAAAAAAAAAAAAAAAAAAAaa!!!!!!!!!'

			if self.actZuk.getFree():
				self.commandLIST()
				for person in self.people.values():
					if person.getSource() == self.actZuk.getPlace():
						self.commandTAKE(person.getId())

			gogo = -1
			print self.getRoads(self.connections[self.actZuk.getPlace()])
			gogo = random.choice(
				[roadId for roadId in self.getRoads(self.connections[self.actZuk.getPlace()]) if self.roads[roadId].getCost() < 100]
				)

			self.commandGO(gogo)
			self.commandWAIT()
			time.sleep(1)
			self.actZuk.visited.append(gogo)

