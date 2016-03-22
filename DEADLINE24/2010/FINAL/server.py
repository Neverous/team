# -*- encoding: utf-8 -*-

import sys
import socket
import select
import logic

BUFFER_SIZE = 16384

class Server(object):
	def __init__(self, address = u'server.dl24', port = 20000):
		u"""
		"""

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.setsockopt(socket.SOL_SOCKET, socket.TCP_NODELAY, 1)
		self.address = address
		self.port = port

	def connect(self):
		u"""
		"""

		self.sock.connect((self.address, self.port))

		return True

	def read(self):
		u"""
		"""

		data = u''
		while not data:
			data = self.recv()

		data = data.strip()
		print u'DBG: RECV: "%s"' % data
		return data

	def recv(self):
		u"""
		"""

		data = u''
		next = u''

		while select.select([self.sock, ], [], [], 0.5)[0]:
			try:
				next = self.sock.recv(BUFFER_SIZE)
				data += next
				if not next:
					break

			except Exception, msg:
				print msg
				return u''

		return unicode(data.strip())

	def write(self, data):
		u"""
		"""

		data += u'\n'
		try:
			if select.select([], [self.sock, ], [], 0.5)[1]:
				self.sock.send(data.encode(u'utf-8'))
				print u'DBG: SEND: "%s"' % data.strip()
				return True

		except Exception, msg:
			print msg
			return False

	def login(self):
		u"""
		"""

		data = self.read()
		if data <> u"LOGIN":
			raise IOError(u"Unknown data: %s" % data)

		self.write("team08") # LOGIN
		data = self.read()
		if data <> u"PASS":
			raise IOError(u"Login failed: %s" % data)

		self.write("ludrulam") # HASŁO
		data = self.read()
		if data <> u"OK":
			raise IOError(u"Login failed: %s" % data)

		print u'DBG: LOGGED IN'
		return True

if __name__ == u'__main__':
	if True:
	#try:
		server = Server(port = int(sys.argv[1]))
		server.connect()
		server.login()
		game = logic.Game(server = server, debug = True, game = int(sys.argv[2]))
		try:
			game.run(int(sys.argv[3]))

		except KeyboardInterrupt:
			pass

	#except Exception, msg:
		#print msg, msg.__class__.__name__

	game.save()
