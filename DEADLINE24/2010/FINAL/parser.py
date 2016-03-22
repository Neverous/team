# -*- encoding: utf-8 -*-

from zuk import Zuk
from person import Person
from town import Town
from road import Road

class Parser(object):
	def parseZUK(self, data):
		data = data.split(u'\n')
		zuk = self.parseZUKInfo(u'\n'.join(data[:2]))
		data = data[2:]
		for u in xrange(zuk.getUsed()):
			person = self.parseZUKPerson(data[u])
			zuk.addPerson(person)

		return zuk

	def parseZUKInfo(self, data):
		zuk = Zuk()
		data = data.split(u'\n')

		first = data[0].split(u' ')
		zuk.setId(int(first[0]))
		zuk.setCapacity(int(first[1]))
		zuk.setFuel(float(first[2]))
		zuk.setUsed(int(first[3]))

		second = data[1].split(u' ')
		if second[0] == u'TOWN':
			zuk.setPlace(int(second[1]))
			zuk.setTimeout(float(second[2]))

		else:
			zuk.setPlace(int(second[2]))
			zuk.setTimeout(float(second[4]))

		return zuk

	def parseZUKPerson(self, data):
		person = Person()
		data = data.split(u' ')
		person.setId(int(data[0]))
		person.setDestination(int(data[1]))
		person.setMoney(float(data[2]))

		return person

	def parseTOWN(self, data):
		data = data.split(u' ')
		id = int(data[0])
		x = float(data[1])
		y = float(data[2])

		return Town(id, x, y)

	def parseROAD(self, data):
		data = data.split(u' ')
		id = int(data[0])
		destination = int(data[1])
		speed = float(data[2])
		cost = float(data[3])
		length = float(data[4])

		return Road(id, None, destination, speed, cost, length)

	def parsePERSON(self, data):
		person = Person()
		data = data.split(u' ')
		person.setId(int(data[0]))
		person.setSource(int(data[1]))
		person.setDestination(int(data[2]))
		person.setMoney(float(data[3]))

		return person
