import unittest
import app

class TestLightController(unittest.TestCase):
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

    def test_ping(self):
        engine = self.MockATEngine()
        controller = app.controller.LightController(engine)

        want = ['PING RESPONSE1', 'PING RESPONSE2']
        engine.results.append(want)
        got = controller.ping('PING REQUEST')
        self.assertEqual(got, ','.join(want))
        self.assertEqual(len(engine.calls), 1)
        self.assertEqual(engine.calls[0]['cmd'], 'PING')
        self.assertEqual(engine.calls[0]['data'], 'PING REQUEST')

    def test_led(self):
        engine = self.MockATEngine()
        controller = app.controller.LightController(engine)

        controller.led('STATE 123')
        self.assertEqual(len(engine.calls), 1)
        self.assertEqual(engine.calls[0]['cmd'], 'LED')
        self.assertEqual(engine.calls[0]['data'], 'STATE 123')