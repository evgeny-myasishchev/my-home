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

class HTTPContext():
    __slots__ = ['method', 'uri', 'httpVersion']
    def __init__(self, input):
        reqLine = _parse_req_line(input.readline())
        self.method = reqLine[0]
        self.uri = reqLine[1]
        self.httpVersion = reqLine[2]
        pass

def _parse_req_line(req_line):
    firstSP = 0
    nextSP = req_line.index(' ', firstSP)
    
    method = req_line[firstSP:nextSP]
    
    firstSP = nextSP+1
    nextSP = req_line.index(' ', firstSP)

    uri = req_line[firstSP:nextSP]
    version = req_line[nextSP+1:]
    if version[-1] == '\n':
        version = version[:-1]
    return (method, uri, version)