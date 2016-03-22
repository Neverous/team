# -*- encoding: utf-8 -*-

class Person(object):
	def __init__(self, id = None, source = None, destination = None, money = None):
		self.id = id
		self.source = source
		self.destination = destination
		self.money = money

	def setId(self, id):
		self.id = id
		return True

	def getId(self):
		return self.id

	def setSource(self, source):
		self.source = source
		return True

	def getSource(self):
		if self.source == None:
			return -1

		return self.source

	def setDestination(self, destination):
		self.destination = destination
		return True

	def getDestination(self):
		return self.destination

	def setMoney(self, money):
		self.money = money
		return True

	def getMoney(self):
		return self.money

	def __unicode__(self):
		return u'Person %d: [%d -> %d] %.2f' % (self.getId(), self.getSource(), self.getDestination(), self.getMoney())

	def __str__(self):
		return self.__unicode__().encode(u'utf-8')

	def __repr__(self):
		return self.__str__()
