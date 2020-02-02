import unittest
import middleware
import uhttp

class MockReq:
    pass

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