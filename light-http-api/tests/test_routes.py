import unittest
import app
import uhttp
import ujson

from tests import mocks

class TestPingHandler(unittest.TestCase):
    def test_handle(self):
        engine = mocks.MockATEngine()
        controller = app.controller.LightController(engine)
        handler = app.routes.create_ping_handler(controller)
        req = mocks.MockReq()
        w = mocks.MockWriter()

        engine.results.append(['PONG'])
        handler(w, req)
        self.assertEqual(engine.calls[0]['cmd'], 'PING')
        self.assertEqual(engine.calls[0]['data'], 'PONG')
        self.assertEqual(w.written_status, uhttp.HTTP_STATUS_OK)
        self.assertEqual(w.written_body, ujson.dumps({'service': 'PONG', 'controller': 'PONG'}))

class TestLedHandler(unittest.TestCase):
    def test_handle(self):
        state = 'NEXT-STATE-123'
        payload = '{ "state": "%s" }' % state;
        engine = mocks.MockATEngine()
        controller = app.controller.LightController(engine)
        handler = app.routes.create_led_handler(controller)
        req = mocks.MockReq(payload=payload)
        w = mocks.MockWriter()

        handler(w, req)
        self.assertEqual(engine.calls[0]['cmd'], 'LED')
        self.assertEqual(engine.calls[0]['data'], state)
        self.assertEqual(w.written_status, uhttp.HTTP_STATUS_OK)
