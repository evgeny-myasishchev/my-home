import unittest
import app
import uhttp
import ujson
import random
import urandom
import utime
import ure

from tests import mocks

urandom.seed(utime.ticks_ms())

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
        self.assertEqual(w.written_status, uhttp.HTTP_STATUS_NO_CONTENT)

class TestGetPinHandler(unittest.TestCase):
    def test_handle(self):
        pinNumber = random.randint(10, 1000)
        wantState = random.randint(10, 1000)
        uri = '/v1/light/pins/%d' % pinNumber

        engine = mocks.MockATEngine()
        engine.results.append([str(wantState)])

        controller = app.controller.LightController(engine)
        handler = app.routes.create_get_pin_handler(controller)
        req = mocks.MockReq(uri=uri)
        w = mocks.MockWriter()

        re = ure.compile(r'.+\/([0-9]+)')
        handler(w, req, re.match(uri))
        self.assertEqual(engine.calls[0]['cmd'], 'PIN?')
        self.assertEqual(engine.calls[0]['data'], str(pinNumber))
        self.assertEqual(w.written_status, uhttp.HTTP_STATUS_OK)
        self.assertEqual(ujson.loads(w.written_body), {"state": wantState})

class TestSetPinHandler(unittest.TestCase):
    def test_handle(self):
        pinNumber = random.randint(10, 1000)
        wantState = random.randint(10, 1000)
        uri = '/v1/light/pins/%d' % pinNumber

        payload = '{ "state": %d }' % wantState;

        engine = mocks.MockATEngine()

        controller = app.controller.LightController(engine)
        handler = app.routes.create_set_pin_handler(controller)
        req = mocks.MockReq(uri=uri, payload=payload)
        w = mocks.MockWriter()

        re = ure.compile(r'.+\/([0-9]+)')
        handler(w, req, re.match(uri))
        self.assertEqual(engine.calls[0]['cmd'], 'PIN')
        self.assertEqual(engine.calls[0]['data'], "%d,%d" % (pinNumber, wantState))
        self.assertEqual(w.written_status, uhttp.HTTP_STATUS_NO_CONTENT)
