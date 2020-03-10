import unittest
import uio
import uhttp
import random
import urandom
import utime
import sys
import uos
import _thread
import usocket
import logger
import ujson

urandom.seed(utime.ticks_ms())

class TestRequest(unittest.TestCase):
    def test_parse_req_line(self):
        expectedMethod = uhttp.HTTP_METHODS[random.randint(0, len(uhttp.HTTP_METHODS)-1)]
        expectedURI = '/some/%s/path?q=%s' % (random.randint(0, 1000), random.randint(0, 1000))
        expectedVersion = ['HTTP/1.0', 'HTTP/1.1', 'HTTP/1.2'][random.randint(0, 2)]

        reqLine = b'%s %s %s\n' % (expectedMethod, expectedURI, expectedVersion)
        (method, uri, httpVersion) = uhttp._parse_req_line(reqLine)
        self.assertEqual(method, expectedMethod)
        self.assertEqual(uri, expectedURI)
        self.assertEqual(httpVersion, expectedVersion)

    def test_parse_headers(self):
        host = "domain%s.com" % random.randint(0, 10000)
        withoutSpace = "value1-%s" % random.randint(0, 10000)
        withMultiSpaces = "value2-%s" % random.randint(0, 10000)
        input = uio.BytesIO((
            "Host: " + host + "\r\n"
            "X-Without-Space:" + withoutSpace + "\r\n"
            "X-With-Multi-Spaces:   " + withMultiSpaces + "\r\n"
            "\r\n"
        ))
        headers = uhttp._parse_headers(input)
        self.assertEqual(len(headers), 3)
        self.assertEqual(host, headers["host"])
        self.assertEqual(withoutSpace, headers["x-without-space"])
        self.assertEqual(withMultiSpaces, headers["x-with-multi-spaces"])

    def test_init(self):
        input = uio.BytesIO((
            'GET /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            'X-Header-1: value1\n'
            'X-Header-2: value2\n'
            '\n'
        ))
        req = uhttp.Request(input)
        self.assertEqual("GET", req.method)
        self.assertEqual("/some-resource?qs=value", req.uri)
        self.assertEqual(req.headers, {
            'host': 'domain.com',
            'x-header-1': 'value1',
            'x-header-2': 'value2'
        })

    def test_bad_method(self):
        input = uio.BytesIO((
            'SOMETHING /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            '\n'
        ))
        thrown = False
        err = None
        try:
            uhttp.Request(input)
        except uhttp.HTTPException as e:
            thrown = True
            err = e
        self.assertTrue(thrown, 'No error thrown')
        self.assertEqual(uhttp.HTTP_STATUS_NOT_IMPLEMENTED, err.status)
        self.assertEqual('Unrecognized method', err.body)

    def test_req_body_no_content_length(self):
        input = uio.BytesIO((
            'POST /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            '\n'
            'body line 1\nbody line 2\n'
        ))
        req = uhttp.Request(input)
        err = None
        try:
            req.body()
        except uhttp.HTTPException as e:
            err = e
        self.assertIsNotNone(err, 'err has not been raised')
        self.assertEqual(uhttp.HTTP_STATUS_LENGTH_REQUIRED, err.status)
        self.assertEqual(uhttp.HTTP_REASON_PHRASE[uhttp.HTTP_STATUS_LENGTH_REQUIRED], err.body)

    def test_req_body_bad_content_length(self):
        input = uio.BytesIO((
            'POST /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            'Content-Length: something\n'
            '\n'
            'body line 1\nbody line 2\n'
        ))
        req = uhttp.Request(input)
        err = None
        try:
            req.body()
        except uhttp.HTTPException as e:
            err = e
        self.assertIsNotNone(err, 'err has not been raised')
        self.assertEqual(uhttp.HTTP_STATUS_LENGTH_REQUIRED, err.status)
        self.assertEqual(uhttp.HTTP_REASON_PHRASE[uhttp.HTTP_STATUS_LENGTH_REQUIRED], err.body)

    def test_req_body(self):
        body = 'body line 1\nbody line 2\n'
        input = uio.BytesIO((
            'POST /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            + 'Content-Length: %d\n\n' % len(body)
            + body
        ))
        req = uhttp.Request(input)
        got_body = req.body().read(1024)
        self.assertEqual(body, got_body.decode())

    def test_req_body_limit_to_len(self):
        body = 'body line 1\nbody line 2\n'
        input = uio.BytesIO((
            'POST /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            + 'Content-Length: %d\n\n' % len(body)
            + body
            + 'something else'
        ))
        req = uhttp.Request(input)
        got_body = req.body().read(1024)
        self.assertEqual(body, got_body.decode())

    def test_req_body_json(self):
        json_payload = ('{\n'
            '"key":"value"'
            '}\n')
        input = uio.BytesIO((
            'POST /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            + 'Content-Length: %d\n\n' % len(json_payload)
            + json_payload
        ))
        req = uhttp.Request(input)
        body = ujson.load(req.body())
        self.assertEqual({'key': 'value'}, body)

    def test_req_body_empty(self):
        input = uio.BytesIO((
            'POST /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            'Content-Length: 0\n'
            '\n'
        ))
        req = uhttp.Request(input)
        body = req.body().read(1024)
        self.assertEqual(b'', body)

class TestResponseWriter(unittest.TestCase):
    def test_write_header_known_status(self):
        output = uio.BytesIO()
        writer = uhttp.ResponseWriter(output)

        writer.write_header(uhttp.HTTP_STATUS_OK)
        output.seek(0)
        status_line = output.readline()
        self.assertEqual(b"HTTP/1.1 200 OK", status_line.rstrip())

        output.seek(0)
        writer.write_header(uhttp.HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE)
        output.seek(0)
        status_line = output.readline()
        self.assertEqual(b"HTTP/1.1 415 Unsupported Media Type", status_line.rstrip())
        self.assertEqual(writer.status, uhttp.HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE)

        output.seek(0)
        writer.write_header(uhttp.HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, 'Custom reason phrase 123321')
        output.seek(0)
        status_line = output.readline()
        self.assertEqual(b"HTTP/1.1 415 Custom reason phrase 123321", status_line.rstrip())

    def test_write_header_unknown_status(self):
        output = uio.BytesIO()
        writer = uhttp.ResponseWriter(output)

        writer.write_header(534, 'Custom Reason Phrase')
        output.seek(0)
        status_line = output.readline()
        self.assertEqual(b"HTTP/1.1 534 Custom Reason Phrase", status_line.rstrip())

        thrown = False
        err = None
        try:
            output.seek(0)
            writer.write_header(534)
        except Exception as e:
            thrown = True
            err = e
        self.assertTrue(thrown)
        self.assertEqual('reason_phrase must be provided', str(err))

    def test_write_header_default_headers(self):
        pythonName = sys.implementation.name
        pythonVersion = '.'.join(map(str, sys.implementation.version))
        want_server = 'uhttp/0.1 %s/%s %s' % (pythonName, pythonVersion, sys.platform)

        output = uio.BytesIO()
        writer = uhttp.ResponseWriter(output)
        self.assertEqual(writer.headers, {
            "Server": want_server,
            "Connection": "close",
        })

        writer.write_header(534, 'Custom Reason Phrase')
        output.write('\n\n')
        output.seek(0)
        output.readline()
        got_headers = uhttp._parse_headers(output)

        self.assertEqual(got_headers, {
            'server': want_server,
            'connection': 'close'
        })

    def test_write_header_custom_headers(self):
        output = uio.BytesIO()
        writer = uhttp.ResponseWriter(output)
        writer.headers['X-Header-1'] = 'value1'
        writer.headers['X-Header-2'] = 'value2'
        writer.write_header(534, 'Custom Reason Phrase')
        output.write('\n\n')
        output.seek(0)
        output.readline()
        got_headers = uhttp._parse_headers(output)

        pythonName = sys.implementation.name
        pythonVersion = '.'.join(map(str, sys.implementation.version))
        self.assertEqual(got_headers, {
            'server': 'uhttp/0.1 %s/%s %s' % (pythonName, pythonVersion, sys.platform),
            'connection': 'close',
            'x-header-1': 'value1',
            'x-header-2': 'value2'
        })

    # TODO: Write header & buffer same time
    def test_write_header_and_buffer(self):
        output = uio.BytesIO()
        writer = uhttp.ResponseWriter(output)

        data = b'This is a buffer that will be written'
        writer.write_header(uhttp.HTTP_STATUS_NOT_FOUND)
        writer.write(data)

        want_len = len(data)
        self.assertEqual(writer.headers["Content-Length"], want_len)
        
        output.seek(0)
        statusLine = output.readline()
        self.assertEqual(statusLine.decode().strip(), 'HTTP/1.1 404 Not Found')
        got_headers = uhttp._parse_headers(output)

        # Default headers should be there
        self.assertTrue('server' in got_headers, 'No server header')
        self.assertTrue('connection' in got_headers, 'No connection header')

        self.assertEqual(got_headers, {
            'server': got_headers['server'],
            'connection': got_headers['connection'],
            'content-length': str(want_len)
        })
        got_data = output.read(want_len)
        self.assertEqual(got_data, data)

    def test_write_only(self):
        output = uio.BytesIO()
        writer = uhttp.ResponseWriter(output)

        data = b'This is a buffer that will be written'
        writer.write(data)

        output.seek(0)
        statusLine = output.readline()
        self.assertEqual(statusLine.decode().strip(), 'HTTP/1.1 200 OK')
        got_headers = uhttp._parse_headers(output)
        want_len = len(data)

        # Default headers should be there
        self.assertTrue('server' in got_headers, 'No server header')
        self.assertTrue('connection' in got_headers, 'No connection header')

        self.assertEqual(got_headers, {
            'server': got_headers['server'],
            'connection': got_headers['connection'],
            'content-length': str(want_len)
        })
        got_data = output.read(want_len)
        self.assertEqual(got_data, data)

class TestHTTPServer(unittest.TestCase):
    def do_req(self, port, method='GET'):
        addr = usocket.getaddrinfo("0.0.0.0", port)[0][-1]
        socket = usocket.socket()
        socket.connect(addr)
        socket.send('%s / HTTP/1.1\r\nHost: localhost\r\n\r\n' % method)
        status_line = socket.readline()
        headers = uhttp._parse_headers(socket)
        body = socket.readline()
        socket.close()
        return (status_line, headers, body)

    def start_server(self, server):
        server.start_async()
        utime.sleep_ms(10) # give a chance for server to start listening

    def test_req_res(self):
        port = random.randint(10000, 30000)
        server = uhttp.HTTPServer(
            handler=lambda w, req: w.write(b'Some response data'),
            port=port,
            logger=logger.TestLogger(),
        )
        self.start_server(server)
        status_line, headers, body = self.do_req(port)
        self.assertEqual(status_line.decode().strip(), 'HTTP/1.1 200 OK')
        self.assertEqual(3, len(headers), 'Expected 4 headers but got: %s' % str(headers))
        self.assertEqual(b'Some response data', body)
        server.stop()

    def test_res_default_status(self):
        port = random.randint(10000, 30000)
        def handler(w, req):
            w.status = uhttp.HTTP_STATUS_CONFLICT
        server = uhttp.HTTPServer(
            handler=handler,
            port=port,
            logger=logger.TestLogger(),
        )
        self.start_server(server)
        status_line, _, _ = self.do_req(port)
        self.assertEqual(
            status_line.decode().strip(), 
            'HTTP/1.1 %s %s' % (uhttp.HTTP_STATUS_CONFLICT, uhttp.HTTP_REASON_PHRASE[uhttp.HTTP_STATUS_CONFLICT])
        )
        server.stop()

    def test_empty_handler(self):
        port = random.randint(10000, 30000)
        server = uhttp.HTTPServer(
            handler=lambda w, req: None,
            port=port,
            logger=logger.TestLogger(),
        )
        self.start_server(server)
        status_line, headers, body = self.do_req(port)
        self.assertEqual(status_line.decode().strip(), 'HTTP/1.1 200 OK')
        self.assertEqual(2, len(headers), 'Expected 2 headers but got: %s' % str(headers))
        self.assertEqual(b'', body)
        server.stop()

    def test_multiple_requests(self):
        port = random.randint(10000, 30000)
        server = uhttp.HTTPServer(
            handler=lambda w, req: w.write('PONG'),
            port=port,
            logger=logger.TestLogger(),
        )
        self.start_server(server)
        status_line, headers, body = self.do_req(port)
        self.assertEqual(status_line.decode().strip(), 'HTTP/1.1 200 OK')
        self.assertEqual(3, len(headers), 'Expected 3 headers but got: %s' % str(headers))
        self.assertEqual(b'PONG', body)
        status_line, headers, body = self.do_req(port)
        self.assertEqual(status_line.decode().strip(), 'HTTP/1.1 200 OK')
        self.assertEqual(3, len(headers), 'Expected 3 headers but got: %s' % str(headers))
        self.assertEqual(b'PONG', body)
        server.stop()

    def test_process_client_handle_parsing_errors(self):
        server = uhttp.HTTPServer(
            handler=lambda w, req: w.write('PONG'),
            port=8080,
            logger=logger.TestLogger(),
        )
        client = uio.BytesIO((
            'SOMETHING /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            '\n'
        ))
        output = uio.BytesIO()
        writer = uhttp.ResponseWriter(output)
        server._process_client(writer, client)

        output.seek(0)
        self.assertEqual(
            output.readline().decode().strip(), 
            'HTTP/1.1 %s %s' % (uhttp.HTTP_STATUS_NOT_IMPLEMENTED, uhttp.HTTP_REASON_PHRASE[uhttp.HTTP_STATUS_NOT_IMPLEMENTED])
        )
        # Scroll to body
        while output.readline() != b'\r\n':
            pass
        self.assertEqual(
            output.readline().decode().strip(), 
            'Unrecognized method'
        )
        server.stop()

    def test_process_client_handle_unhandled_handler_errors(self):
        def handler(w, req):
            raise Exception('Something happened')
        server = uhttp.HTTPServer(
            handler=handler,
            port=8080,
            logger=logger.TestLogger(),
        )
        client = uio.BytesIO((
            'GET /some-resource?qs=value HTTP/1.1\n'
            'Host: domain.com\n'
            '\n'
        ))
        output = uio.BytesIO()
        writer = uhttp.ResponseWriter(output)
        server._process_client(writer, client)

        output.seek(0)
        self.assertEqual(
            output.readline().decode().strip(), 
            'HTTP/1.1 %s %s' % (uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR, uhttp.HTTP_REASON_PHRASE[uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR])
        )
        server.stop()
