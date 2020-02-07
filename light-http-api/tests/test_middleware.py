import unittest
import middleware
import uhttp
import uio
from uuid import uuid4

class MockReq(uhttp.Request):
    def __init__(self, *, 
        method="GET", 
        host="example.com", 
        uri="/v1/something",
        userAgent="test-middleware/v0"
    ):
        uhttp.Request.__init__(self, uio.BytesIO((
            b"%s %s HTTP/1.1\n"
            b"Host: %s\n"
            b"User-Agent: %s\n\n" % (method, uri, host, userAgent)
        )))

class MockWriter:
    def __init__(self):
        self.written_status = None
        self.written_body = None

    def write_header(self, status):
        self.written_status = status

    def write(self, body):
        self.written_body = body

class TestNotFound(unittest.TestCase):
    def test_responds_with_404(self):
        req = MockReq()
        writer = MockWriter()
        middleware.not_found(None)(writer, req)
        self.assertEqual(writer.written_status, uhttp.HTTP_STATUS_NOT_FOUND)

class TestRecover(unittest.TestCase):
    def test_call_next(self):
        req = MockReq()
        writer = MockWriter()
        next_called = False
        def next_mw(w, r):
            nonlocal next_called
            next_called = True
        middleware.recover(next_mw)(writer, req)
        self.assertTrue(next_called)

    def test_respond_with_500_if_error(self):
        err = Exception("Failed to handle request")
        req = MockReq()
        writer = MockWriter()
        def next_mw(w, r):
            raise err
        middleware.recover(next_mw)(writer, req)
        self.assertEqual(writer.written_status, uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR)
        self.assertEqual(writer.written_body, uhttp.HTTP_REASON_PHRASE[uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR])

    def test_respond_with_error_details_if_debug(self):
        err = Exception("Failed to handle request")
        req = MockReq()
        writer = MockWriter()
        def next_mw(w, r):
            raise err
        middleware.recover(next_mw, debug=True)(writer, req)
        self.assertEqual(writer.written_status, uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR)
        self.assertEqual(writer.written_body, str(err))

class TestTrace(unittest.TestCase):
    def test_call_next(self):
        req = MockReq()
        writer = MockWriter()
        next_called = False
        def next_mw(w, r):
            nonlocal next_called
            next_called = True
        middleware.trace(next_mw)(writer, req)
        self.assertTrue(next_called)

    def test_inject_new_req_id_to_context(self):
        req = MockReq()
        req_id = uuid4()

        writer = MockWriter()
        next_called = False
        def next_mw(w, r):
            nonlocal next_called
            next_called = True
        middleware.trace(next_mw, uuid_fn=lambda: req_id)(writer, req)
        self.assertEqual(req.context['requestId'], req_id)

    def test_inject_req_id_from_header(self):
        req = MockReq()
        req_id = uuid4()
        req.headers['x-request-id'] = req_id

        writer = MockWriter()
        next_called = False
        def next_mw(w, r):
            nonlocal next_called
            next_called = True
        middleware.trace(next_mw)(writer, req)
        self.assertEqual(req.context['requestId'], req_id)

    def test_log_requests(self):
        req = MockReq(
            method="POST", 
            host="logs.example.com", 
            uri="/v1/something/logs",
            userAgent="Test-Logs V1"
        )
        req_id = uuid4()
        req.headers["x-request-id"] = req_id
        req.context["requestId"] = req_id
        req.context["something-else"] = "value 123"

        class MockLogger:
            def __init__(self):
                self.info_logs = []
            def info(self, msg, context=None, data=None, err=None):
                self.info_logs.append({
                    "msg": msg,
                    "context": context,
                    "data": data,
                    "err": err
                })
                pass

        mock_logger = MockLogger()

        writer = MockWriter()
        next_called = False
        def next_mw(w, r):
            nonlocal next_called
            next_called = True
        middleware.trace(next_mw, logger=mock_logger)(writer, req)
        self.assertEqual(len(mock_logger.info_logs), 2)
        self.assertEqual(mock_logger.info_logs[0], {
            "msg": "BEGIN REQ: POST /v1/something/logs",
            "context": req.context,
            "data": {
                "method": req.method,
                "host": req.headers["host"],
                "uri": req.uri,
                "userAgent": req.headers["user-agent"]
            },
            "err": None
        })
        self.assertEqual(mock_logger.info_logs[1], {
            "msg": "END REQ",
            "context": req.context,
            "data": None,
            "err": None
        })