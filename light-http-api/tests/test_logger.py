import unittest
import logger
import random
import urandom
import utime
import ujson

urandom.seed(utime.ticks_ms())

class TestJsonFormatter(unittest.TestCase):
    def test_format_with_msg(self):
        msg = 'Something is happening %d' % random.randint(10, 1000)
        level = 'info'
        now = 'time %d' % random.randint(10, 1000)
        want_message = ujson.dumps({
            'msg': msg,
            'time': now,
            'level': level,
            'v': 1
        })
        got_message = logger.json_formatter(msg=msg, level=level, now=now)
        self.assertEqual(got_message, want_message)

    def test_format_with_data(self):
        msg = 'Hello'
        level = 'info'
        now = 'now'
        data = {
            'key1': 'val1',
            'key2': 'val2',
            'key3': 'val3',
        }
        want_payload = {
            'msg': msg,
            'time': now,
            'level': level,
            'v': 1,
        }
        want_payload['data'] = data
        got_message = logger.json_formatter(msg=msg, level=level, now=now, data=data)
        self.assertEqual(got_message, ujson.dumps(want_payload))

    def test_format_with_error(self):
        msg = 'Hello'
        level = 'info'
        now = 'now'
        err = Exception('Some error')
        want_payload = {
            'msg': msg,
            'time': now,
            'level': level,
            'v': 1,
        }
        want_payload['err'] = err
        got_message = logger.json_formatter(msg=msg, level=level, now=now, err=err)
        self.assertEqual(got_message, ujson.dumps(want_payload))


class TestRFC3339Now(unittest.TestCase):
    def test_format_localtime(self):
        localtime = (
            2020,
            10,
            20,
            12,
            30,
            59,
            0,
            0
        )
        want = '2020-10-20T12:30:59Z'
        got = logger.rfc_3339_now(lambda: localtime)
        self.assertEqual(got, want)

    def test_pad_values(self):
        localtime = (
            2020,
            1,
            2,
            3,
            4,
            5,
            0,
            0
        )
        want = '2020-01-02T03:04:05Z'
        got = logger.rfc_3339_now(lambda: localtime)
        self.assertEqual(got, want)

class TestSetupLogger(unittest.TestCase):
    def test_write_msg(self):
        class target():
            pass
        err = Exception('Test error')
        test_data = {"key1": 'val1', "key2": 'val2'}
        now = '2020-01-02T03:04:05Z'

        got_messages = []
        def mock_transport(message):
            got_messages.append(message)

        logger.setup(
            target=target,
            transport=mock_transport,
            now_fn=lambda: now,
        )

        target.error('Hello Error', data=test_data, err=err)
        target.warn('Hello Warn', data=test_data, err=err)
        target.info('Hello Info', data=test_data, err=err)
        target.debug('Hello Debug', data=test_data, err=err)

        self.assertEqual(got_messages, [
            logger.json_formatter(
                msg='Hello Error', level='error', now=now, data=test_data, err=err
            ),
            logger.json_formatter(
                msg='Hello Warn', level='warn', now=now, data=test_data, err=err
            ),
            logger.json_formatter(
                msg='Hello Info', level='info', now=now, data=test_data, err=err
            ),
            logger.json_formatter(
                msg='Hello Debug', level='debug', now=now, data=test_data, err=err
            ),
        ])