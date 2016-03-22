# -*- encoding: utf-8 -*-

class Town(object):
	def __init__(self, id = None, x = None, y = None):
		self.id = id
		self.x = x
		self.y = y

	def setId(self, id):
		self.id = id
		return True

	def getId(self):
		return self.id

	def setPos(self, x, y):
		self.x = x
		self.y = y
		return True

	def getPos(self):
		return self.x, self.y

	def __unicode__(self):
		return u'Town: %d(%d, %d)' % (self.getId(), self.x, self.y)

	def __str__(self):
		return self.__unicode__().encode(u'utf-8')

	def __repr__(self):
		return self.__str__()
