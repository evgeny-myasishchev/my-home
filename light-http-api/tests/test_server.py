import unittest
import uio
import server
import random
import urandom
import utime

urandom.seed(utime.ticks_ms())

class TestHTTPContext(unittest.TestCase):
    def test_parse_req_line(self):
        methods = ['GET', 'HEAD', 'POST', 'PUT', 'DELETE', 'CONNECT', 'OPTIONS', 'TRACE', 'PATCH']
        expectedMethod = methods[random.randint(0, len(methods)-1)]
        expectedURI = '/some/%s/path?q=%s' % (random.randint(0, 1000), random.randint(0, 1000))
        expectedVersion = ['HTTP/1.0', 'HTTP/1.1', 'HTTP/1.2'][random.randint(0, 2)]

        reqLine = "%s %s %s\n" % (expectedMethod, expectedURI, expectedVersion)
        (method, uri, httpVersion) = server._parse_req_line(reqLine)
        self.assertEqual(method, expectedMethod)
        self.assertEqual(uri, expectedURI)
        self.assertEqual(httpVersion, expectedVersion)

    def test_parse_headers(self):
        host = "domain%s.com" % random.randint(0, 10000)
        withoutSpace = "value1-%s" % random.randint(0, 10000)
        withMultiSpaces = "value2-%s" % random.randint(0, 10000)
        input = uio.StringIO((
            "Host: " + host + "\n"
            "X-Without-Space:" + withoutSpace + "\n"
            "X-With-Multi-Spaces:   " + withMultiSpaces + "\n"
            "\n"
        ))
        headers = server._parse_headers(input)
        self.assertEqual(len(headers), 3)
        self.assertEqual(host, headers["host"])
        self.assertEqual(withoutSpace, headers["x-without-space"])
        self.assertEqual(withMultiSpaces, headers["x-with-multi-spaces"])
        pass