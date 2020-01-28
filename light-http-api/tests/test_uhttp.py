import unittest
import uio
import uhttp
import random
import urandom
import utime

urandom.seed(utime.ticks_ms())

class TestRequest(unittest.TestCase):
    def test_parse_req_line(self):
        methods = ['GET', 'HEAD', 'POST', 'PUT', 'DELETE', 'CONNECT', 'OPTIONS', 'TRACE', 'PATCH']
        expectedMethod = methods[random.randint(0, len(methods)-1)]
        expectedURI = '/some/%s/path?q=%s' % (random.randint(0, 1000), random.randint(0, 1000))
        expectedVersion = ['HTTP/1.0', 'HTTP/1.1', 'HTTP/1.2'][random.randint(0, 2)]

        reqLine = "%s %s %s\n" % (expectedMethod, expectedURI, expectedVersion)
        (method, uri, httpVersion) = uhttp._parse_req_line(reqLine)
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
        headers = uhttp._parse_headers(input)
        self.assertEqual(len(headers), 3)
        self.assertEqual(host, headers["host"])
        self.assertEqual(withoutSpace, headers["x-without-space"])
        self.assertEqual(withMultiSpaces, headers["x-with-multi-spaces"])

    def test_init(self):
        input = uio.StringIO((
            "GET /some-resource?qs=value HTTP/1.1\n"
            "Host: domain.com\n"
            "X-Header-1: value1\n"
            "X-Header-2: value2\n"
            "\n"
        ))
        req = uhttp.Request(input)
        self.assertEqual("GET", req.method)
        self.assertEqual("/some-resource?qs=value", req.uri)
        pass