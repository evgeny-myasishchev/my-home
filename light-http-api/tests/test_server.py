import unittest
import uio
import server
import random
import urandom
import utime

class TestHTTPContext(unittest.TestCase):
    def test_parse_req_line(self):
        urandom.seed(utime.ticks_ms())
        methods = ['GET', 'HEAD', 'POST', 'PUT', 'DELETE', 'CONNECT', 'OPTIONS', 'TRACE', 'PATCH']
        expectedMethod = methods[random.randint(0, len(methods)-1)]
        expectedURI = '/some/%s/path?q=%s' % (random.randint(0, 1000), random.randint(0, 1000))
        expectedVersion = ['HTTP/1.0', 'HTTP/1.1', 'HTTP/1.2'][random.randint(0, 2)]

        reqLine = "%s %s %s\n" % (expectedMethod, expectedURI, expectedVersion)
        (method, uri, httpVersion) = server._parse_req_line(reqLine)
        self.assertEqual(method, expectedMethod)
        self.assertEqual(uri, expectedURI)
        self.assertEqual(httpVersion, expectedVersion)