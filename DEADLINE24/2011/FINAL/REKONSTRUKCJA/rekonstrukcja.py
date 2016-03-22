#-*- encoding: utf-8 -*-
import os
import random
import time
import threading
import sys
from deadline24 import Server
from exceptions import Exception
import Image, ImageDraw, ImageColor

PORT = int(sys.argv[1])

rotator = {
	0: 0,
	90: 270,
	180: 180,
	270: 90,
}

##########################################################################

def unpack(d24, name, password):
	d24.debug('%s %s' % (name, password))
	if os.system("unrar x -p%s -y obrazy/%s.rar unpacked/ > /dev/null" % (password, name)):
		raise RuntimeError(u"Unrar got fucked up!")

	if os.system("mkdir -p unpacked/%s > /dev/null" % name):
		raise RuntimeError(u"Mkdir got fucked up!")

	return True

def divide(d24, name, rows, cols):
	im = Image.open("unpacked/%s.jpg" % name)
	Image.new('RGB', im.size).save("unpacked/%s-act.jpg" % name)
	width = im.size[0] / cols
	height = im.size[1] / rows
	for r in range(rows):
		for c in xrange(cols):
			box = im.crop((c * width, r * height, (c + 1) * width, (r + 1) * height))
			box.save("unpacked/%s/%s-%d-%d.jpg" % (name, name, r + 1, c + 1))

	d24.debug('Probably divided!')
	return (im.size[0], im.size[1])

def emptyGrid(rows, cols):
	grid = []
	for r in xrange(rows):
		grid.append([[r, c, 0, 0] for c in xrange(cols)])

	return grid

def emptyGrid2(rows, cols):
	grid = []
	for r in xrange(rows):
		grid.append([True for c in xrange(cols)])

	return grid


###########################################################################

class Guesser(threading.Thread):
	def __init__(self, rows, cols, piece):
		super(Guesser, self).__init__()
		self._rows = rows
		self._cols = cols
		self._piece = piece
		self._d24 = Server(PORT, 'Guesser')
		self._d24.connect()
		self._d24.login()
		self.stopit = False

	def run(self):
		self._d24.debug('Running!')
		while not self.stopit:
			try:
				data = open('GUESS%d' % PORT, 'r').read()
				os.system('rm GUESS%d' % PORT)

			except Exception, msg:
				self._d24.debug(unicode(msg))
				From = []
				To = []
				free = emptyGrid2(rows, cols)
				for r in xrange(rows):
					for c in xrange(cols):
						if self._piece[r][c][2]:
							free[self._piece[r][c][0]][self._piece[r][c][1]] = False

						else:
							From.append((r, c))

				for r in xrange(rows):
					for c in xrange(cols):
						if free[r][c]:
							To.append((r, c))

				self._d24.debug('Z: %d, Do: %d' % (len(To), len(From)))
				a = random.choice(From)
				b = random.choice(To)
				data = "%d %d %d %d %d" % (a[1] + 1, a[0] + 1, b[1] + 1, b[0] + 1, random.choice((0, 90, 180, 270)))

			try:
				self._d24.command("GUESS " + data)

			except Exception, msg:
				self._d24.debug(unicode(msg))

		self._d24.debug('Closed!')

class Drawer(threading.Thread):
	def __init__(self, name, piece, d24):
		super(Drawer, self).__init__()
		self._name = name
		self._piece = piece
		self._d24 = d24
		self.stopit = False

	def run(self):
		self._d24.debug('Running!')
		orig = Image.open("unpacked/%s.jpg" % self._name)
		draw = ImageDraw.Draw(orig)
		size = orig.size
		res = Image.open("unpacked/%s-act.jpg" % self._name)
		while not self.stopit:
			for r, row in enumerate(self._piece):
				for c, cell in enumerate(row):
					if not cell[2]:
						continue

					im = Image.open("unpacked/%s/%s-%d-%d.jpg" % (self._name, self._name, r + 1, c + 1))
					cwidth, cheight = im.size
					im = im.rotate(rotator[cell[3]])
					res.paste(im, (cell[1] * cwidth, cell[0] * cheight, (cell[1] + 1) * cwidth, (cell[0] + 1) * cheight))
					draw.rectangle(((c * cwidth, r * cheight), ((c + 1) * cwidth, (r + 1) * cheight)), ImageColor.getcolor('black', 'RGB'))

			res.save("unpacked/%s-act.jpg" % self._name)
			orig.save("unpacked/%s.jpg" % self._name)
			self._d24.debug('Drawed')
			time.sleep(2)

		self._d24.debug('Closed!')

class Builder(threading.Thread):
	def __init__(self, name, piece):
		super(Builder, self).__init__()
		self._name = name
		self._piece = piece
		self._d24 = Server(PORT, 'Builder')
		self._d24.connect()
		self._d24.login()

	def run(self):
		self._d24.debug('Running!')
		while True:
			resp = self._d24.command("CHANGES").split('\n')
			if resp[0] != self._name:
				break

			good = int(resp[2])
			for g in xrange(good):
				col, row, dcol, drow, drot, count = resp[3 + g].split()
				col = int(col) - 1
				row = int(row) - 1
				dcol = int(dcol) - 1
				drow = int(drow) - 1
				drot = int(drot)
				count = int(count[1:-1])
				self._piece[row][col] = (drow, dcol, count, drot)

		self._d24.debug('Closed!')

##########################################################################

d24 = Server(PORT, 'Global')

d24.connect()
d24.login()

d24.debug('SZCZESCLIWY NUMEREK: %d' % os.getpid())
while True:
	if True:
	#try:
		# RUNDA
		name, password, cols, rows = d24.command("INFO").split();
		cols = int(cols)
		rows = int(rows)
		if password == "[WillShowOnNextTurn]":
			d24.command('WAIT')
			continue

		unpack(d24, name, password)
		width, height = divide(d24, name, rows, cols)

		piece = emptyGrid(rows, cols)

		build = Builder(name, piece)
		guess = Guesser(rows, cols, piece)
		draw = Drawer(name, piece, d24)

		build.start()
		guess.start()
		draw.start()

		build.join()
		guess.stopit = True
		draw.stopit = True
		guess.join()
		draw.join()

	#except Exception, msg:
		#d24.debug(unicode(msg))
