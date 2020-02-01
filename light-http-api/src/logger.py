import sys
import ujson

_levels = {
    'error' : 50,
    'warn' : 40, 
    'info' : 30, 
    'debug' : 20,
}

def rfc_3339_now():
    pass

# class UDPLogger:
#     def __init__(self, host, port):
#         import usocket
#         self._addr = usocket.getaddrinfo(host, port, usocket.AF_INET6, usocket.SOCK_DGRAM)[0][-1]
#         self._socket = usocket.socket(usocket.AF_INET6, usocket.SOCK_DGRAM)

#     def log(self, msg):
#         self._socket.sendto(msg, self._addr)

# class PrintLogger():
#     def log(self, msg):
#         print(msg)


# def createFormatter():
#     def micropythonFormatter(msg):
#         now = utime.localtime()
#         return "[%d-%02d-%02d %02d:%02d:%02d] %s" % (now[0], now[1], now[2], now[3], now[4], now[5], msg)

#     # def pythonFormatter(msg):
#     #     return msg
#     import utime

#     return micropythonFormatter

# _logger = None
# _formatter = createFormatter()

def json_formatter(*, msg, level, now, data=None, err=None):
    payload = {
        "msg": msg,
        "time": now,
        "level": level,
        "v": 1,
    }
    if data != None:
        payload['data'] = data
    if err != None:
        payload['err'] = err
    return ujson.dumps(payload)

def print_transport(message):
    pass

# _formatter = jsonFormatter
# _transport = None

# Setup level logger functions
# for level in _levels:
#     setattr(thismodule, level, lambda *args, **kwargs: _log(*args, **kwargs, level=level))

thismodule = sys.modules[__name__]

# now_fn should return 
def setup(*, target=thismodule, transport=print_transport, formatter=json_formatter, now_fn=rfc_3339_now):
    pass
