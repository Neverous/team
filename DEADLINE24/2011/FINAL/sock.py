import socket
import threading
import SocketServer

class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):

	def handle(self):
		data = self.request.recv(1024)
		cur_thread = threading.currentThread()
		response = "%s: %s" % (cur_thread.getName(), data)
		self.request.send(response)

def client(ip, port, message):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
	sock.connect((ip, port))
	sock.send(message)
	response = sock.recv(1024)
	print "Received: %s" % response
	sock.close()

if __name__ == "__main__":
	HOST, PORT = "localhost", 0

	server = SocketServer.ThreadingTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
	ip, port = server.server_address

	server_thread = threading.Thread(target=server.serve_forever)
	server_thread.setDaemon(True)
	server_thread.start()
	print "Server loop running in thread:", server_thread.getName()

	client(ip, port, "Hello World 1")
	client(ip, port, "Hello World 2")
	client(ip, port, "Hello World 3")

	client(ip, port, "Hello World 1")
	client(ip, port, "Hello World 2")
	client(ip, port, "Hello World 3")

	server.shutdown()
