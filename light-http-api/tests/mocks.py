import uhttp
import uio

class MockReq(uhttp.Request):
    def __init__(self, *, 
        method="GET", 
        host="example.com", 
        uri="/v1/something",
        userAgent="test-middleware/v0",
        payload=None
    ):
        reqData = (
            "%s %s HTTP/1.1\n"
            "Host: %s\n"
            "User-Agent: %s\n" % (method, uri, host, userAgent)
        )
        if payload != None:
            reqData += 'Content-Length: %d\n\n' % len(payload)
            reqData += payload
        reqData += '\n'

        input = uio.BytesIO(reqData.encode())
        uhttp.Request.__init__(self, input)

class MockWriter(uhttp.ResponseWriter):
    def __init__(self):
        output = uio.BytesIO()
        uhttp.ResponseWriter.__init__(self, output)
        self.mock_output = output
        self.written_status = None
        self.written_body = None

    def write_header(self, status):
        uhttp.ResponseWriter.write_header(self, status)
        self.written_status = status

    def write(self, body):
        uhttp.ResponseWriter.write(self, body)
        self.written_body = body

class MockATEngine:
    def __init__(self):
        self.results = []
        self.calls = []
        pass

    def dispatch_command(self, cmd, data=None, *, context=None):
        self.calls.append({
            "cmd": cmd,
            "data": data,
        })
        if len(self.results) > 0:
            return self.results.pop(0)
        return []