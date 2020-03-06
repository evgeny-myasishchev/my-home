import usocket
import sys
import logger
import errno

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

HTTP_METHODS = [
    'CONNECT',
    'DELETE',
    'GET',
    'HEAD',
    'OPTIONS',
    'PATCH',
    'POST',
    'PUT',
    'TRACE'
]

class HTTPException(Exception):
    def __init__(self, status, body):
        self.status = status
        self.body = body
        pass

class HTTPServer:
    def __init__(self, 
        *,
        handler,
        host='0.0.0.0',
        port, 
        logger=logger,
        maxClients = 5):
        self._host = host
        self._port = port
        self._logger = logger
        self._maxClients = maxClients
        self._handler = handler
        self._socket = usocket.socket()
        self._socket.setsockopt(usocket.SOL_SOCKET, usocket.SO_REUSEADDR, 1)

    def start(self): 
        addr = usocket.getaddrinfo(self._host, self._port, 0, usocket.SOCK_STREAM)
        self._socket.bind(addr[0][-1])
        self._socket.listen(5)
        self._logger.info("Server started on port: %s" % self._port)

        while True:
            try:
                client, addr = self._socket.accept()
            except OSError as err:
                # Server probably stopped
                if err.args[0] == errno.ECONNABORTED:
                    return
                raise err
            req = Request(client)
            writer = ResponseWriter(client)
            self._handler(writer, req)
            if not writer._header_sent:
                writer.write_header(writer.status)
            client.close()

    def start_async(self):
        import _thread
        _thread.start_new_thread(lambda srv: srv.start(), (self,))

    def stop(self):
        self._socket.close()
        self._logger.info('Server stopped')

class Request():
    __slots__ = [
        'method', 
        'uri', 
        'httpVersion',
        'headers',
        'context',
    ]
    def __init__(self, input):
        reqLine = _parse_req_line(input.readline())

        self.method = reqLine[0]
        if self.method not in HTTP_METHODS:
            raise HTTPException(HTTP_STATUS_NOT_IMPLEMENTED, 'Unrecognized method')

        self.uri = reqLine[1]
        self.httpVersion = reqLine[2]
        self.headers = _parse_headers(input)
        self.context = {}

def _parse_req_line(req_line):
    # TODO: Validate and throw appropriate error
    req_line = req_line.decode()
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
    nextHeaderLine = input.readline().decode('utf-8')
    while nextHeaderLine != '' and nextHeaderLine != '\n' and nextHeaderLine != '\r\n':
        pairs = nextHeaderLine.split(':', 1)
        headers[pairs[0].lower()] = pairs[1].strip(' \r\n')
        nextHeaderLine = input.readline().decode('utf-8')
    return headers


SERVER_FINGERPRINT='uhttp/0.1 %s/%s %s' % (
    sys.implementation.name, 
    '.'.join(map(str, sys.implementation.version)), 
    sys.platform)

class ResponseWriter():
    def __init__(self, output):
        self._output = output
        self.status = HTTP_STATUS_OK
        self.headers = {
            'Server': SERVER_FINGERPRINT,

            # For now we do not support keep-alive connections
            'Connection': 'close',
        }
        self._header_sent = False

    def write_header(self, status, reason_phrase = None):
        self.status = status

        # Status line
        self._output.write('HTTP/1.1 ')
        self._output.write(str(status))
        self._output.write(' ')
        if reason_phrase == None and status in HTTP_REASON_PHRASE:
            reason_phrase = HTTP_REASON_PHRASE[status]

        if reason_phrase == None:
            raise Exception('reason_phrase must be provided')

        self._output.write(reason_phrase)
        self._output.write('\r\n')
        for key, val in self.headers.items():
            self._output.write(key)
            self._output.write(': ')
            self._output.write(val)
            self._output.write('\r\n')
        self._header_sent = True

    def write(self, buf):
        content_length = len(buf)
        if not self._header_sent:
            self.write_header(HTTP_STATUS_OK)
        self.headers["Content-Length"] = content_length
        self._output.write('Content-Length: ')
        self._output.write(str(content_length))
        self._output.write('\r\n')
        self._output.write('\r\n')
        self._output.write(buf)
