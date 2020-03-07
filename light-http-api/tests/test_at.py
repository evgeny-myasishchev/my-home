import unittest
import uio
import at

class TestATEngine(unittest.TestCase):
    class TestCMD(at.ATCommand):
        def __init__(self, name):
            super().__init__(name)
            self.responseProcessed = False

        def process_response(self, response):
            self.responseProcessed = True
            return response

    def test_dispatch_command(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = self.TestCMD("CMD1")
        input.write('OK\n')
        input.seek(0)
        resp = engine.dispatch_command(cmd)
        output.seek(0)
        cmd_line = output.readline()
        self.assertEqual(cmd_line, b'AT+CMD1\n')
        self.assertEqual(resp.status, 'OK')
        self.assertEqual(resp.data, [])

    def test_dispatch_command_with_error(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = self.TestCMD("CMD1")
        input.write('ERROR\n')
        input.seek(0)
        resp = engine.dispatch_command(cmd)
        output.seek(0)
        cmd_line = output.readline()
        self.assertEqual(cmd_line, b'AT+CMD1\n')
        self.assertEqual(resp.status, 'ERROR')
        self.assertEqual(resp.data, [])

    def test_dispatch_command_without_any_response(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = self.TestCMD("CMD1")
        input.seek(0)
        resp = engine.dispatch_command(cmd)
        self.assertEqual(resp.status, 'ERROR')
        self.assertEqual(resp.status_message, 'Response loop limit reached')

    def test_dispatch_command_with_input_data(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = self.TestCMD("CMD1")
        input.write('OK\n')
        input.seek(0)
        resp = engine.dispatch_command(cmd, "SOME-DATA-123")
        output.seek(0)
        cmd_line = output.readline()
        self.assertEqual(cmd_line, b'AT+CMD1=SOME-DATA-123\n')
        self.assertEqual(resp.status, 'OK')
        self.assertEqual(resp.data, [])

    def test_dispatch_command_with_response_data(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = self.TestCMD("CMD1")
        input.write('RESULT1\n')
        input.write('RESULT2\n')
        input.write('RESULT3\n')
        input.write('OK\n')
        input.seek(0)
        resp = engine.dispatch_command(cmd)
        self.assertEqual(resp.status, 'OK')
        self.assertEqual(resp.data, ['RESULT1', 'RESULT2', 'RESULT3'])