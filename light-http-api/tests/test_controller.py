import app
import at
import random
import unittest
import urandom
import utime
from tests import mocks

urandom.seed(utime.ticks_ms())

class TestLightController(unittest.TestCase):
    def test_ping(self):
        engine = mocks.MockATEngine()
        controller = app.controller.LightController(engine)

        want = ['PING RESPONSE1', 'PING RESPONSE2']
        engine.results.append(want)
        got = controller.ping('PING REQUEST')
        self.assertEqual(got, ','.join(want))
        self.assertEqual(len(engine.calls), 1)
        self.assertEqual(engine.calls[0]['cmd'], 'PING')
        self.assertEqual(engine.calls[0]['data'], 'PING REQUEST')

    def test_led(self):
        engine = mocks.MockATEngine()
        controller = app.controller.LightController(engine)

        controller.led('STATE 123')
        self.assertEqual(len(engine.calls), 1)
        self.assertEqual(engine.calls[0]['cmd'], 'LED')
        self.assertEqual(engine.calls[0]['data'], 'STATE 123')

    def test_get_pin(self):
        engine = mocks.MockATEngine()
        controller = app.controller.LightController(engine)

        want_pin = random.randint(10, 50)
        want_state = random.randint(50, 100)
        engine.results.append([str(want_state)])

        got_state = controller.get_pin(want_pin)
        self.assertEqual(len(engine.calls), 1)
        self.assertEqual(engine.calls[0]['cmd'], 'PIN?')
        self.assertEqual(engine.calls[0]['data'], str(want_pin))
        self.assertEqual(got_state, want_state)

    def test_get_pin_bad_state(self):
        engine = mocks.MockATEngine()
        controller = app.controller.LightController(engine)

        engine.results.append(['bad1', 'bad2'])

        err = None
        try:
            controller.get_pin(123)
        except at.ATException as e:
            err = e
        self.assertIsNotNone(err)
        self.assertEqual(err.args[0], 'Unexpected response')

    def test_set_pin(self):
        engine = mocks.MockATEngine()
        controller = app.controller.LightController(engine)

        want_pin = random.randint(10, 50)
        want_state = random.randint(50, 100)
        
        controller.set_pin(want_pin, want_state)
        self.assertEqual(len(engine.calls), 1)
        self.assertEqual(engine.calls[0]['cmd'], 'PIN')
        self.assertEqual(engine.calls[0]['data'], "%d,%d" % (want_pin, want_state))