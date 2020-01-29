import usocket
import logger

HTTP_STATUS_OK=200
HTTP_STATUS_CREATED=201
HTTP_STATUS_ACCEPTED=202
HTTP_STATUS_NO_CONTENT=204
HTTP_STATUS_MOVED_PERMANENTLY=301
HTTP_STATUS_FOUND=302
HTTP_STATUS_BAD_REQUEST=400
HTTP_STATUS_UNAUTHORIZED=401
HTTP_STATUS_FORBIDDEN=403
HTTP_STATUS_NOT_FOUND=404
HTTP_STATUS_METHOD_NOT_ALLOWED=405
HTTP_STATUS_CONFLICT=409
HTTP_STATUS_LENGTH_REQUIRED=411
HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE=415
HTTP_STATUS_INTERNAL_SERVER_ERROR=500
HTTP_STATUS_NOT_IMPLEMENTED=501
HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED=505

HTTP_REASON_PHRASE = {
    HTTP_STATUS_OK: 'OK',
    HTTP_STATUS_CREATED: 'Created',
    HTTP_STATUS_ACCEPTED: 'Accepted',
    HTTP_STATUS_NO_CONTENT: 'No Content',
    HTTP_STATUS_MOVED_PERMANENTLY: 'Moved Permanently',
    HTTP_STATUS_FOUND: 'Found',
    HTTP_STATUS_BAD_REQUEST: 'Bad Request',
    HTTP_STATUS_UNAUTHORIZED: 'Unauthorized',
    HTTP_STATUS_FORBIDDEN: 'Forbidden',
    HTTP_STATUS_NOT_FOUND: 'Not Found',
    HTTP_STATUS_METHOD_NOT_ALLOWED: 'Method Not Allowed',
    HTTP_STATUS_CONFLICT: 'Conflict',
    HTTP_STATUS_LENGTH_REQUIRED: 'Length Required',
    HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE: 'Unsupported Media Type',
    HTTP_STATUS_INTERNAL_SERVER_ERROR: 'Internal Server Error',
    HTTP_STATUS_NOT_IMPLEMENTED: 'Not Implemented',
    HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED: 'HTTP Version Not Supported'
}

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

class Request():
    __slots__ = [
        'method', 
        'uri', 
        'httpVersion'
    ]
    def __init__(self, input):
        reqLine = _parse_req_line(input.readline())
        self.method = reqLine[0]
        self.uri = reqLine[1]
        self.httpVersion = reqLine[2]

def _parse_req_line(req_line):
    # TODO: Validate and throw appropriate error
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

def _parse_headers(input):
    # TODO: Fail if bad header line
    # TODO: Maybe limit number of headers
    # TODO: Multiple values?
    # Review spec https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html

    headers = {}
    nextHeaderLine = input.readline()
    while nextHeaderLine != '\n':
        pairs = nextHeaderLine.split(':', 1)
        headers[pairs[0].lower()] = pairs[1].strip(' \n')
        nextHeaderLine = input.readline()
    return headers

class ResponseWriter():
    def __init__(self, output):
        self._output = output
        self.headers = {
            'Connection': 'close' # For now we do not support keep-alive connections
        }

    def write_header(self, status, reason_phrase = None):
        # Status line
        self._output.write('HTTP/1.1 ')
        self._output.write(str(status))
        self._output.write(' ')
        if reason_phrase == None and status in HTTP_REASON_PHRASE:
            reason_phrase = HTTP_REASON_PHRASE[status]

        if reason_phrase == None:
            raise Exception('reason_phrase must be provided')

        self._output.write(reason_phrase)
        pass