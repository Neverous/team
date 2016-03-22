# -*- encoding: utf-8 -*-

from person import Person

class Zuk(object):
	def __init__(self, id = None, capacity = None, fuel = None, used = None, place = None, timeout = None):
		self.id = id
		self.capacity = capacity
		self.fuel = fuel
		self.used = used
		self.place = place
		self.timeout = timeout
		self.people = {}
		self.heading = None
		self.visited = []

	def addPerson(self, person):
		self.people[person.getId()] = person
		self.used = len(self.people.values())
		return True

	def purgePeople(self):
		removed = False
		for id, person in self.people.items():
			if person.getDestination() == self.getPlace():
				del self.people[id]
				removed = True

		self.used = len(self.people)
		return removed

	def setId(self, id):
		self.id = id
		return True

	def getId(self):
		return self.id

	def setCapacity(self, capacity):
		self.capacity = capacity
		return True

	def getCapacity(self):
		return self.capacity

	def setFuel(self, fuel):
		self.fuel = fuel
		return True

	def getFuel(self):
		return self.fuel

	def setUsed(self, used):
		self.used = used
		return True

	def getUsed(self):
		return self.used

	def setPlace(self, place):
		self.place = place
		return True

	def getPlace(self):
		return self.place

	def setTimeout(self, timeout):
		self.timeout = timeout
		return True

	def getTimeout(self):
		return self.timeout

	def getFree(self):
		return self.getCapacity() - self.getUsed()

	def headingFor(self):
		return self.heading

	def setHeading(self, personId):
		self.heading = personId
		return True

	def __unicode__(self):
		return u'Zuk %d: [%d] %.2f $/km %d/%d p avail: %.2f\n%s\nV: %s' % (
			self.getId(),
			self.getPlace(),
			self.getFuel(),
			self.getUsed(),
			self.getCapacity(),
			self.getTimeout(),
			u'\n'.join([u'\t* ' + unicode(person) for person in self.people.values()]),
			u', '.join([unicode(v) for v in self.visited]),
		)

	def __str__(self):
		return self.__unicode__().encode(u'utf-8')

	def __repr__(self):
		return self.__str__()

