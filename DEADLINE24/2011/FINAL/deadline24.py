# -*- encoding: utf-8 -*-

import sys
import socket
import select
import time

BUFFER_SIZE = 16384
HOST = 'universum.dl24'

class Server(object):
	def __init__(self, port = 20000, debug = 'UNKNOWN', address = HOST):
		u"""
		"""

		self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self._sock.setsockopt(socket.SOL_SOCKET, socket.TCP_NODELAY, 1)
		self._address = address
		self._port = port
		self._debug = debug

	def connect(self):
		u"""
		"""

		self._sock.connect((self._address, self._port))
		return True

	def read(self):
		u"""
		"""

		data = u''
		while not data:
			data = self.recv()

		data = data.strip()
		self.debug('RECV: "%s"' % data)
		return data

	def debug(self, msg):
		print u'[%s]%s: %s' % (time.strftime('%H:%M:%S'), self._debug, msg)
		return True

	def recv(self):
		u"""
		"""

		data = u''
		next = u''

		while select.select([self._sock, ], [], [], 0.5)[0]:
			try:
				next = self._sock.recv(BUFFER_SIZE)
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

		data += u'\r\n'
		try:
			if select.select([], [self._sock, ], [], 0.5)[1]:
				self._sock.send(data.encode(u'utf-8'))
				self.debug('SEND: "%s"' % data.strip())
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

		self.write("team8") # LOGIN
		data = self.read()
		if data <> u"PASS":
			raise IOError(u"Login failed: %s" % data)

		self.write("zeecxczb") # HASŁO
		data = self.read()
		if data <> u"OK":
			raise IOError(u"Login failed: %s" % data)

		self.debug('LOGGED IN')
		return True

	def wait(self, seconds = 1):
		time.sleep(seconds)
		return True

	def command(self, data):
		self.write(data)
		resp = self.read().split(u'\n')
		if resp[0] != u'OK':
			raise IOError(u"Command failed: %s" % resp[0])

		return '\n'.join(resp[1:])

	def Wait(self):
		res = self.command('WAIT')
		if not res.endswith('OK'):
			return self.read()

		return res
