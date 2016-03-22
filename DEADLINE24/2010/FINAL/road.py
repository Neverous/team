# -*- encoding: utf-8 -*-

class Road(object):
	def __init__(self, id = None, start = None, end = None, speed = None, cost = None, length = None):
		self.id = id
		self.start = start
		self.end = end
		self.speed = speed
		self.cost = cost
		self.length = length

	def setId(self, id):
		self.id = id
		return True

	def getId(self):
		return self.id

	def setStart(self, start):
		self.start = start
		return True

	def getStart(self):
		return self.start

	def setEnd(self, end):
		self.end = end
		return True

	def getEnd(self):
		return self.end

	def setSpeed(self, speed):
		self.speed = speed
		return True

	def getSpeed(self):
		return self.speed

	def setLength(self, length):
		self.length = length
		return True

	def getLength(self):
		return self.length

	def setCost(self, cost):
		self.cost = cost
		return True

	def getCost(self):
		return self.cost

	def __unicode__(self):
		return u'Road: %d(%d, %d) %.2f m/s %.2f m %.2f$' % (self.getId(), self.getStart(), self.getEnd(), self.getSpeed(), self.getLength(), self.getCost())

	def __str__(self):
		return self.__unicode__().encode(u'utf-8')

	def __repr__(self):
		return self.__str__()

if __name__ == u'__main__':
	droga = Droga(0, 1, 2, 1, 100, 100)
	print droga
