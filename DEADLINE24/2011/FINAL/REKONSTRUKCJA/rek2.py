#-*- encoding: utf-8 -*-
import os
import random
import time
import threading
import sys
import deadline24
from deadline24 import Server
from exceptions import Exception
import Image, ImageDraw, ImageColor

PORT = int(sys.argv[1])
HOST = 'universum.dl24'

memory = {}

rotator = {
	0: 0,
	90: 270,
	180: 180,
	270: 90,
}

##########################################################################

#def sumL(a, b):
	#res = []
	#for i in xrange(3):
		#res.append(a[i] + b[i])
#
	#return res
#
#def divL(ll, dd):
	#res = []
	#for i in ll:
		#res.append(i / dd)
#
	#return res

def avgPix(img, args):
	#cc = 0
	#pixel = [0, 0, 0]
	return img.getpixel(args[0])

	#for i in xrange(3):
		#try:
			#pixel = sumL(pixel, img.getpixel(args[i]))
			#cc += 1
#
		#except:
			#pass
#
	#pixel = divL(pixel, cc)
	#return tuple(pixel)

def unpack(d24, name, password):
	d24.debug('%s %s' % (name, password))
	if os.system("unrar x -p%s -y obrazy/%s.rar unpacked/ > /dev/null" % (password, name)):
		raise RuntimeError(u"Unrar got fucked up!")

	if os.system("mkdir -p unpacked/%s > /dev/null" % name):
		raise RuntimeError(u"Mkdir got fucked up!")

	return True

def divide(d24, name, rows, cols):
	if memory.has_key(name):
		im = memory[name]

	else:
		im = memory[name] = Image.open("unpacked/%s.jpg" % name)

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
		grid.append([None for c in xrange(cols)])

	return grid

def getEdge(name, image, edge):
	res = []
	if memory.has_key('%s-%d-%d-%d-%d' % (name, image[0] + 1, image[1] + 1, image[2], edge)):
		return list(memory['%s-%d-%d-%d-%d' % (name, image[0] + 1, image[1] + 1, image[2], edge)])

	if memory.has_key('%s-%d-%d-%d' % (name, image[0] + 1, image[1] + 1, image[2])):
		im = memory['%s-%d-%d-%d' % (name, image[0] + 1, image[1] + 1, image[2])]

	else:
		im = memory['%s-%d-%d-%d' % (name, image[0] + 1, image[1] + 1, image[2])] = Image.open('unpacked/%s/%s-%d-%d.jpg' % (name, name, image[0] + 1, image[1] + 1)).rotate(rotator[image[2]])

	width, height = im.size

	if edge == 0:
		for i in xrange(height - 1, -1, -1):
			res.append(avgPix(im, ((0, i), (0, i - 1), (0, i + 1))))

	if edge == 1:
		for i in xrange(width):
			res.append(avgPix(im, ((i, 0), (i - 1, 0), (i + 1, 0))))

	if edge == 2:
		for i in xrange(height):
			res.append(avgPix(im, ((width - 1, i), (width - 1, i - 1), (width - 1, i + 1))))

	if edge == 3:
		for i in xrange(width - 1, -1, -1):
			res.append(avgPix(im, ((i, height - 1), (i - 1, height - 1), (i + 1, height - 1))))

	if edge == 4:
		res = [[], [], [], []]

		for i in xrange(height - 1, -1, -1):
			res[0].append(avgPix(im, ((0, i), (0, i - 1), (0, i + 1))))

		for i in xrange(width):
			res[1].append(avgPix(im, ((i, 0), (i - 1, 0), (i + 1, 0))))

		for i in xrange(height):
			res[2].append(avgPix(im, ((width - 1, i), (width - 1, i - 1), (width - 1, i + 1))))

		for i in xrange(width - 1, -1, -1):
			res[3].append(avgPix(im, ((i, height - 1), (i - 1, height - 1), (i + 1, height - 1))))

	else:
		xx = memory['%s-%d-%d-%d-%d' % (name, image[0] + 1, image[1] + 1, image[2], edge)] = list(reversed(res))
		return xx

	xx = memory['%s-%d-%d-%d-%d' % (name, image[0] + 1, image[1] + 1, image[2], edge)] = res
	return xx

def markIt(name, r, c, rot, edge):
	ones = getEdge(name, (r, c, rot), 4)
	mark = [1000000, 1000000, 1000000, 1000000]
	for e in xrange(4):
		if not edge[e]:
			continue

		mark[e] = 0
		for p in xrange(0, len(edge), 1):
			mark[e] += abs(edge[e][p][0] - ones[e][p][0]) + abs(edge[e][p][1] - ones[e][p][1]) + abs(edge[e][p][2] - ones[e][p][2])

	return sum(mark) / 4

def checkNeighbours(name, mapa, avail):
	print 'DBG: started %d' % len(avail)
	start = time.time()
	av = len(avail)
	checked = 0
	count = 0
	best = [1000000000, 1000000000, 1000000000, 100000000, 1000000000, 1000000000]
	for r, row in enumerate(mapa):
		for c, D in enumerate(row):
			if D != False and not (D == None and av < 128):
				continue

			mapa[r][c] = None
			edge = [None, None, None, None]
			if r and mapa[r - 1][c]:
				edge[1] = getEdge(name, mapa[r - 1][c], 3)

			if c and mapa[r][c - 1]:
				edge[0] = getEdge(name, mapa[r][c - 1], 2)

			if c + 1 < len(row) and mapa[r][c + 1]:
				edge[2] = getEdge(name, mapa[r][c + 1], 0)

			if r + 1 < len(mapa) and mapa[r + 1][c]:
				edge[3] = getEdge(name, mapa[r + 1][c], 1)

			if not edge[0] and not edge[1] and not edge[2] and not edge[3]:
				continue

			count = edge.count(None)
			if count == 4:
				continue

			elif count == 3:
				count = 256

			elif count == 2:
				count = 512

			else:
				count = 1024

			checked += 1
			for (R, C) in avail[:min(av, count)]:
				for rot in xrange(4):
					mark = markIt(name, R, C, rot * 90, edge)
					if mark < best[0]:
						best = [mark, R, C, r, c, rot * 90]

	end = time.time()
	print 'DBG: Avail %d checked %d time %d' % (len(avail), checked, end - start)
	if best[0] < 1000000000:
		return (best[1], best[2]), (best[3], best[4]), best[5]

	return None, None, None

###########################################################################

class Guesser(threading.Thread):
	def __init__(self, rows, cols, piece, piece2, name, special):
		super(Guesser, self).__init__()
		self._rows = rows
		self._cols = cols
		self._piece = piece
		self._piece2 = piece2
		self._name = name
		self._special = special
		self._d24 = Server(PORT, 'Guesser', HOST)
		self._d24.connect()
		self._d24.login()
		self.stopit = False

	def run(self):
		self._d24.debug('Running!')
		last = (0, 0, 0)
		while not self.stopit:
			From = []
			To = []
			for r in xrange(rows):
				for c in xrange(cols):
					if not self._piece[r][c][2]:
						From.append((r, c))

					if not self._piece2[r][c]:
						To.append((r, c))

			self._d24.debug('ZOSTALO: %d' % len(From))
			random.shuffle(From)
			self._special[0] = True
			a, b, rot = checkNeighbours(self._name, self._piece2, From)
			if a:
				a = tuple(a)

			if b:
				b = tuple(b)

			self._special[0] = False
			if a and b:
				self._d24.debug('HEURA: %d %d %d %d %d' % (a[1] + 1, a[0] + 1, b[1] + 1, b[0] + 1, rot))

			if not a or not b or (last[0] == a and last[1] == b and last[2] == rot):
				self._d24.debug('Z: %d, Do: %d' % (len(To), len(From)))
				a = random.choice(From)
				b = random.choice(To)
				rot = random.choice((0, 90, 180, 270))

			else:
				last = (a, b, rot)

			data = "%d %d %d %d %d" % (a[1] + 1, a[0] + 1, b[1] + 1, b[0] + 1, rot)

			try:
				self._d24.command("GUESS " + data)

			except Exception, msg:
				self._d24.debug(unicode(msg))

			time.sleep(0.6)

		self._d24.debug('Closed!')

class Guesser2(threading.Thread):
	def __init__(self, rows, cols, piece, piece2, name, special):
		super(Guesser2, self).__init__()
		self._rows = rows
		self._cols = cols
		self._piece = piece
		self._piece2 = piece2
		self._name = name
		self._special = special
		self._d24 = Server(PORT, 'GuesserRandomHeura', HOST)
		self._d24.connect()
		self._d24.login()
		self.stopit = False

	def run(self):
		self._d24.debug('Running!')
		last = (0, 0, 0)
		while not self.stopit:
			From = []
			To = []
			for r in xrange(rows):
				for c in xrange(cols):
					if not self._piece[r][c][2]:
						From.append((r, c))

					if not self._piece2[r][c]:
						To.append((r, c))

			self._d24.debug('ZOSTALO: %d' % len(From))
			self._special[1] = True
			a, b, rot = checkNeighbours(self._name, self._piece2, random.sample(From, min(len(From), 16)))
			self._special[1] = False

			if a and b:
				self._d24.debug('HEURA: %d %d %d %d %d' % (a[1] + 1, a[0] + 1, b[1] + 1, b[0] + 1, rot))

			if not a or not b or (last[0] == a and last[1] == b and last[2] == rot):
				self._d24.debug('Z: %d, Do: %d' % (len(To), len(From)))
				a = random.choice(From)
				b = random.choice(To)
				rot = random.choice((0, 90, 180, 270))

			else:
				last = (a, b, rot)

			data = "%d %d %d %d %d" % (a[1] + 1, a[0] + 1, b[1] + 1, b[0] + 1, rot)

			try:
				if self._special[0]:
					self._d24.command("GUESS " + data)

			except Exception, msg:
				self._d24.debug(unicode(msg))

			self._d24.Wait()

		self._d24.debug('Closed!')

class Guesser3(threading.Thread):
	def __init__(self, rows, cols, piece, piece2, name, special):
		super(Guesser3, self).__init__()
		self._rows = rows
		self._cols = cols
		self._piece = piece
		self._piece2 = piece2
		self._name = name
		self._special = special
		self._d24 = Server(PORT, 'GuesserRandom', HOST)
		self._d24.connect()
		self._d24.login()
		self.stopit = False

	def run(self):
		self._d24.debug('Running!')
		last = (0, 0, 0)
		while not self.stopit:
			From = []
			To = []
			for r in xrange(rows):
				for c in xrange(cols):
					if not self._piece[r][c][2]:
						From.append((r, c))

					if not self._piece2[r][c]:
						To.append((r, c))

			self._d24.debug('ZOSTALO: %d' % len(From))
			self._d24.debug('Z: %d, Do: %d' % (len(To), len(From)))
			a = random.choice(From)
			b = random.choice(To)
			rot = random.choice((0, 90, 180, 270))

			data = "%d %d %d %d %d" % (a[1] + 1, a[0] + 1, b[1] + 1, b[0] + 1, rot)

			try:
				if self._special[0]:
					self._d24.command("GUESS " + data)

			except Exception, msg:
				self._d24.debug(unicode(msg))

			self._d24.Wait()

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
	def __init__(self, name, piece, piece2):
		super(Builder, self).__init__()
		self._name = name
		self._piece = piece
		self._piece2 = piece2
		self._d24 = Server(PORT, 'Builder', HOST)
		self._d24.connect()
		self._d24.login()

	def run(self):
		self._d24.debug('Running!')
		while True:
			resp = self._d24.command("CHANGES").split('\n')
			if resp[0] != self._name or not int(resp[1]):
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
				self._piece2[drow][dcol] = (row, col, drot)
				if drow and not self._piece2[drow - 1][dcol]:
					self._piece2[drow - 1][dcol] = False

				if dcol and not self._piece2[drow][dcol - 1]:
					self._piece2[drow][dcol -  1] = False

				if dcol + 1 < len(self._piece2[0]) and not self._piece2[drow][dcol + 1]:
					self._piece2[drow][dcol + 1] = False

				if drow + 1 < len(self._piece2) and not self._piece2[drow + 1][dcol]:
					self._piece2[drow + 1][dcol] = False

		self._d24.debug('Closed!')

##########################################################################
#class x:
	#def debug(self, x):
		#print x

#d24 = x()
#mapa = [
	#[(0, 0, 0), (0, 1, 0), None, None],
	#[(1, 0, 0), None, None, None],
	#[None, None, None, None],
	#[None, None, None, None],
	#]

#divide(d24, 'gowno', 4, 4)
#
#print checkNeighbours('gowno', mapa, ((0, 2), (0, 3), (1, 1), (1, 2), (1, 3), (2, 0), (2, 1), (2, 2), (2, 3), (3, 0), (3, 1), (3, 2), (3, 3)))
#
#
#sys.exit(0)

d24 = Server(PORT, 'Global', HOST)

d24.connect()
d24.login()

d24.debug('SZCZESCLIWY NUMEREK: %d' % os.getpid())
while True:
	try:
		memory.clear()
		# RUNDA
		name, password, cols, rows = d24.command("INFO").split();
		cols = int(cols)
		rows = int(rows)
		if password == "[WillShowOnNextTurn]":
			d24.Wait()
			continue

		unpack(d24, name, password)
		width, height = divide(d24, name, rows, cols)

		piece = emptyGrid(rows, cols)
		piece2 = emptyGrid2(rows, cols)

		special = [False, False]
		build = Builder(name, piece, piece2)
		guess = Guesser(rows, cols, piece, piece2, name, special)
		#guess2 = Guesser2(rows, cols, piece, piece2, name, special)
		guess3 = Guesser3(rows, cols, piece, piece2, name, special)
		draw = Drawer(name, piece, d24)

		build.start()
		guess.start()
		#guess2.start()
		guess3.start()
		draw.start()

		build.join()
		guess.stopit = True
		#guess2.stopit = True
		guess3.stopit = True
		draw.stopit = True
		guess.join()
		#guess2.join()
		guess3.join()
		draw.join()

	except Exception, msg:
		d24.debug(unicode(msg))
		d24.read()
