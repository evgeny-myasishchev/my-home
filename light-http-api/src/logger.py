import sys
import ujson
import utime
import usocket
import uio

_levels = [
    'error',
    'warn',
    'info',
    'debug',
]

RFC_3339_UTC_FORMAT='%d-%02d-%02dT%02d:%02d:%02dZ'

def rfc_3339_now(localtime=utime.localtime):
    now = localtime()
    return RFC_3339_UTC_FORMAT % (
        now[0],
        now[1],
        now[2],
        now[3],
        now[4],
        now[5],
    )

def json_formatter(*, msg, level, now, context=None, data=None, err=None):
    payload = {
        "msg": msg,
        "time": now,
        "level": level,
        "v": 1,
    }
    if context != None:
        payload['context'] = context
    if data != None:
        payload['data'] = data
    if err != None:
        err_data = None
        if err != None:
            err_data = uio.StringIO()
            sys.print_exception(err, err_data)
            err_data.seek(0)
            err_data = err_data.read().strip().split('\n')
        payload['err'] = err_data[-1]
        payload['trace'] = err_data[:-1]
    return ujson.dumps(payload)

def print_transport(message):
    print(message)

def make_combined_transport(*transports):
    def firehose(message):
        for transport in transports:
            try:
                transport(message)
            except:
                pass
    return firehose

def make_udp_transport(host, port, broadcast=False):
    addr = None
    socket = usocket.socket(usocket.AF_INET, usocket.SOCK_DGRAM)
    if broadcast:
        socket.setsockopt(usocket.SOL_SOCKET, usocket.SO_BROADCAST, 1)
    def send(message):
        nonlocal addr
        if addr == None:
            try:
                addr = usocket.getaddrinfo(host, port, usocket.AF_INET, usocket.SOCK_DGRAM)[0][-1]
            except:
                return
        socket.sendto(message + "\n", addr)
    return send

def make_file_transport(path):
    output = uio.open(path, 'wb')
    def send(message):
        output.write(message)
        output.write('\n')
    return send

# Setup level logger functions
def make_log(*, level, now_fn, transport):
    def log(msg, *, context=None, data=None, err=None):
        message = json_formatter(
            msg=msg, 
            level=level, 
            now=now_fn(), 
            context=context, 
            data=data, 
            err=err)
        transport(message)
    return log

# Logging methods
# To be actually defined by setup function
def error(msg, context=None, data=None, err=None):
    pass
def warn(msg, context=None, data=None, err=None):
    pass
def info(msg, context=None, data=None, err=None):
    pass
def debug(msg, context=None, data=None, err=None):
    pass

class TestLogger:
    def __init__(self):
        info_logs = []
        self.info_logs = info_logs
        for level in _levels:
            def method(msg, context=None, data=None, err=None):
                info_logs.append({
                    "msg": msg,
                    "context": context,
                    "data": data,
                    "err": err
                })
            setattr(self, level, method)

# TODO: Log levels
def setup(*,
    target=__import__(__name__),
    transport=print_transport,
    formatter=json_formatter, 
    now_fn=rfc_3339_now):
    for level in _levels:
        setattr(target, level, make_log(level=level, now_fn=now_fn, transport=transport))