import unittest
import uio
import at

class TestATEngine(unittest.TestCase):
    def test_dispatch_command(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = "CMD1"
        input.write('OK\n')
        input.seek(0)
        resp = engine.dispatch_command(cmd)
        output.seek(0)
        cmd_line = output.readline()
        self.assertEqual(cmd_line, b'AT+CMD1\n')
        self.assertEqual(resp, [])

    def test_dispatch_command_with_error(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = "CMD1"
        input.write('ERROR\n')
        input.seek(0)
        err = None
        try:
            engine.dispatch_command(cmd)
        except at.ATException as e:
            err = e
        self.assertIsNotNone(err, 'Error had not been raised')
        self.assertIsInstance(err, at.ATException)
        output.seek(0)
        cmd_line = output.readline()
        self.assertEqual(cmd_line, b'AT+CMD1\n')

    def test_dispatch_command_with_string_response(self):
        input = uio.StringIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = "CMD1"
        input.write('OK\n')
        input.seek(0)
        resp = engine.dispatch_command(cmd)
        output.seek(0)
        cmd_line = output.readline()
        self.assertEqual(cmd_line, b'AT+CMD1\n')
        self.assertEqual(resp, [])

    def test_dispatch_command_without_any_response(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = "CMD1"
        input.seek(0)
        err = None
        try:
            engine.dispatch_command(cmd)
        except at.ATException as e:
            err = e
        self.assertIsNotNone(err, 'Error had not been raised')
        self.assertEqual(err.args[0], 'Response loop limit reached')

    def test_dispatch_with_none_response(self):
        class none_input:
            def readline(self):
                return None
        input = none_input()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = "CMD1"
        err = None
        try:
            engine.dispatch_command(cmd)
        except at.ATException as e:
            err = e
        self.assertIsNotNone(err, 'Error had not been raised')
        self.assertEqual(err.args[0], 'Response loop limit reached')

    def test_dispatch_command_with_input_data(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = "CMD1"
        input.write('OK\n')
        input.seek(0)
        resp = engine.dispatch_command(cmd, "SOME-DATA-123")
        output.seek(0)
        cmd_line = output.readline()
        self.assertEqual(cmd_line, b'AT+CMD1=SOME-DATA-123\n')
        self.assertEqual(resp, [])

    def test_dispatch_command_with_response_data(self):
        input = uio.BytesIO()
        output = uio.BytesIO()
        engine = at.ATEngine(input, output)

        cmd = "CMD1"
        input.write('RESULT1\n')
        input.write('RESULT2\n')
        input.write('RESULT3\n')
        input.write('OK\n')
        input.seek(0)
        resp = engine.dispatch_command(cmd)
        self.assertEqual(resp, ['RESULT1', 'RESULT2', 'RESULT3'])