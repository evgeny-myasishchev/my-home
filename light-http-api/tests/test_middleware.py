import unittest
import middleware
import uhttp
import uio
import logger
import ure
import uerrno
from uuid import uuid4
from .mocks import MockReq, MockWriter

class TestUse(unittest.TestCase):
    def test_use_wrap_handler(self):
        req = MockReq()
        writer = MockWriter()
        calls_order = []
        req.context["calls_order"] = calls_order

        def create_mw(arg):
            def mw(next):
                def mw_handler(w, req):
                    req.context["calls_order"].append(arg)
                    next(w, req)
                return mw_handler
            return mw

        def handler(w, req):
            req.context["calls_order"].append("handler")

        middleware.use(handler, 
            create_mw("mw1"), 
            create_mw("mw2"), 
            create_mw("mw3"),
        )(writer, req)

        self.assertEqual(calls_order, ['mw1', 'mw2', 'mw3', 'handler'])

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

    def test_respond_with_408_if_timeout(self):
        err = OSError(uerrno.ETIMEDOUT)
        req = MockReq()
        writer = MockWriter()
        def next_mw(w, r):
            raise err
        middleware.recover(next_mw)(writer, req)
        self.assertEqual(writer.written_status, uhttp.HTTP_STATUS_REQUEST_TIMEOUT)
        self.assertEqual(writer.written_body, uhttp.HTTP_REASON_PHRASE[uhttp.HTTP_STATUS_REQUEST_TIMEOUT])

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
        self.assertEqual(req.context['requestId'], str(req_id))

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
        class MockGC:
            def __init__(self):
                self.mock_mem_alloc = 0
                self.mock_mem_free = 0
            def mem_alloc(self, mock_mem_alloc=None):
                if mock_mem_alloc == None:
                    return self.mock_mem_alloc
                self.mock_mem_alloc = mock_mem_alloc
            def mem_free(self, mock_mem_free=None):
                if mock_mem_free == None:
                    return self.mock_mem_free
                self.mock_mem_free = mock_mem_free


        req = MockReq(
            method="POST", 
            host="logs.example.com", 
            uri="/v1/something/logs",
            userAgent="Test-Logs V1"
        )
        req_id = uuid4()
        req.headers["x-request-id"] = str(req_id)
        req.context["requestId"] = str(req_id)
        req.context["something-else"] = "value 123"

        mock_logger = logger.TestLogger()

        mock_gc = MockGC()
        mem_alloc_before = 101
        mem_alloc_after = 102
        mem_free_before = 103
        mem_free_after = 104

        mock_gc.mem_alloc(mem_alloc_before)
        mock_gc.mem_free(mem_free_before)

        writer = MockWriter()
        next_called = False
        def next_mw(w, r):
            nonlocal next_called
            next_called = True
            mock_gc.mem_alloc(mem_alloc_after)
            mock_gc.mem_free(mem_free_after)
        middleware.trace(next_mw, logger=mock_logger, gc=mock_gc)(writer, req)
        self.assertEqual(len(mock_logger.info_logs), 2)
        self.assertEqual(mock_logger.info_logs[0], {
            "msg": "BEGIN REQ: POST /v1/something/logs",
            "context": req.context,
            "data": {
                "method": req.method,
                "host": req.headers["host"],
                "uri": req.uri,
                "userAgent": req.headers["user-agent"],
                "headers": req.headers,
                "memoryUsage": "(mem_alloc: %s, mem_free: %s)" % (mem_alloc_before, mem_free_before),
            },
            "err": None
        })
        self.assertEqual(mock_logger.info_logs[1], {
            "msg": "END REQ: %s" % writer.status,
            "context": req.context,
            "data": {
                "status": writer.status,
                "headers": writer.headers,
                "memoryUsage": "(mem_alloc: %s, mem_free: %s)" % (mem_alloc_after, mem_free_after),
            },
            "err": None
        })

class TestRouter(unittest.TestCase):
    def test_string_routes(self):
        req = MockReq()

        next_called = False
        def next_mw(w, r):
            nonlocal next_called
            next_called = True

        calls = []
        def ping_handler(w, req):
            nonlocal calls
            calls.append('ping')

        def time_handler(w, req):
            nonlocal calls
            calls.append('time')

        router = middleware.create_router([
            ('GET', '/ping', ping_handler),
            ('GET', '/time', time_handler)
        ])(next_mw)

        req.uri = "/ping"
        router(None, req)
        self.assertEqual(calls, ["ping"])
        calls = []

        req.uri = "/ping?something"
        router(None, req)
        self.assertEqual(calls, ["ping"])
        self.assertEqual(next_called, False)
        calls = []

        req.uri = "/time"
        router(None, req)
        self.assertEqual(calls, ["time"])
        self.assertEqual(next_called, False)
        calls = []

        req.uri = "/v1/time"
        router(None, req)
        self.assertEqual(calls, [])
        self.assertEqual(next_called, True)
        calls = []
        next_called = False

    def test_route_methods(self):
        req = MockReq()

        def next_mw(w, r):
            pass

        calls = []
        def get_ping_handler(w, req):
            nonlocal calls
            calls.append('get-ping')

        def post_ping_handler(w, req):
            nonlocal calls
            calls.append('post-ping')

        router = middleware.create_router([
            ('GET', '/ping', get_ping_handler),
            ('POST', '/ping', post_ping_handler)
        ])(next_mw)
        
        req.uri = "/ping"
        router(None, req)
        self.assertEqual(calls, ["get-ping"])
        calls = []

        req.method = 'POST'
        router(None, req)
        self.assertEqual(calls, ["post-ping"])
        calls = []

    def test_re_routes(self):
        req = MockReq()

        next_called = False
        def next_mw(w, r):
            nonlocal next_called
            next_called = True

        calls = []
        def users_create_handler(w, req, match):
            nonlocal calls
            calls.append(('users_create_handler', match.group(1)))

        def user_transactions_handler(w, req, match):
            nonlocal calls
            calls.append(('user_transactions_handler', match.group(1), match.group(2)))

        router = middleware.create_router([
            ('GET', ure.compile(r"/v1/users/([A-Za-z0-9\-]+)/create"), users_create_handler),
            ('GET', ure.compile(r"/v1/users/([A-Za-z0-9\-]+)/transactions/([A-Za-z0-9\-]+)"), user_transactions_handler),
        ])(next_mw)

        user1 = str(uuid4())
        req.uri = "/v1/users/%s/create" % user1
        router(None, req)
        self.assertEqual(calls, [("users_create_handler", user1)])
        self.assertEqual(next_called, False)
        calls = []

        trx1 = str(uuid4())
        req.uri = "/v1/users/%s/transactions/%s" % (user1, trx1)
        router(None, req)
        self.assertEqual(calls, [("user_transactions_handler", user1, trx1)])
        self.assertEqual(next_called, False)
        calls = []

        trx1 = str(uuid4())
        req.uri = "/v1/users/%s/update" % (user1)
        router(None, req)
        self.assertEqual(calls, [])
        self.assertEqual(next_called, True)
        calls = []

