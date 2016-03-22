# -*- encoding: utf-8 -*-

import sys
import socket
import select
import time

BUFFER_SIZE = 16384
HOST = 'universum.dl24'
#HOST = 'localhost'
USER = 'team22'
PASSWORD = 'pdjzvqceyh'

class CommandError(IOError): pass

class Server(object):
	def __init__(self, port = 20000, debug = 'UNKNOWN', address = HOST, user = USER, password = PASSWORD):
		self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		#self._sock.setsockopt(socket.SOL_SOCKET, socket.TCP_NODELAY, 1)
		self._address = address
		self._port = port
		self._debug = debug
		self._user = user
		self._password = password
		self._out = open('/tmp/%s' % debug, 'w')

	def connect(self):
		self._sock.connect((self._address, self._port))
		return True

	def read(self):
		data = u''
		while not data:
			data = self.recv()

		data = data.strip()
		self.debug('RECV: "%s"' % data)
		return data

	def debug(self, msg):
		print >>self._out, u'[%s]%s: %s' % (time.strftime('%H:%M:%S'), self._debug, msg)
		#print >>sys.stderr, u'[%s]%s: %s' % (time.strftime('%H:%M:%S'), self._debug, msg)
		self._out.flush()
		return True

	def recv(self):
		data = u''
		next = u''
		while select.select([self._sock, ], [], [], 0.1)[0]:
			try:
				next = self._sock.recv(BUFFER_SIZE)
				data += next
				if not next:
					break

			except Exception, msg:
				self.debug('Recv exception: ' + str(msg))
				return u''

		return unicode(data.strip())

	def write(self, data):
		data += u'\r\n'
		try:
			if select.select([], [self._sock, ], [], 0.1)[1]:
				self._sock.send(data.encode(u'utf-8'))
				self.debug('SEND: "%s"' % data.strip())
				return True

		except Exception, msg:
			self.debug('Send exception: ' + str(msg))
			return False

	def login(self):
		data = self.read()
		if data <> u"LOGIN":
			raise IOError(u"Unknown data: %s" % data)

		self.write(self._user)
		data = self.read()
		if data <> u"PASS":
			raise IOError(u"Login failed: %s" % data)

		self.write(self._password) # HASŁO
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
		resp = self.read().split('\n')
		if resp[0] != u'OK':
			res = resp[0].split()
			if res[0] == 'FAILED' and int(res[1]) == 7:
				if not resp[-1].endswith('OK'):
					self.read()

				return self.command(data)

			if res[0] != 'FAILED' or int(res[1]) != 6:
				raise CommandError(u"Command failed: %s" % resp[0])

		return '\n'.join(resp[1:])

	def Wait(self):
		res = self.command('WAIT')
		if not res.endswith('OK'):
			return self.read()

		return res
