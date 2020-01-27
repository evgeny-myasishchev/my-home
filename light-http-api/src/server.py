import usocket
import logger

class HTTPServer:
    def __init__(self, 
        port, 
        log,
        maxClients = 5):
        self._port = port
        self._log = log
        self._maxClients = maxClients
        self._socket = usocket.socket()
        pass

    def start(self): 
        addr = usocket.getaddrinfo('127.0.0.1', self._port, 0, usocket.SOCK_STREAM)
        self._socket.bind(addr[0][-1])
        self._socket.listen(addr[0][-1])
        self._log("Server started on port: %s" % self._port)

        client, addr = self._socket.accept()
        self._log(addr)


def start(port=80, log=logger.log):
    server = HTTPServer(port, log)
    server.start()

logger.setupPrintLogger()

start(8080)