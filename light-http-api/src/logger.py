class UDPLogger:
    def __init__(self, host, port):
        import usocket
        self._addr = usocket.getaddrinfo(host, port, usocket.AF_INET6, usocket.SOCK_DGRAM)[0][-1]
        self._socket = usocket.socket(usocket.AF_INET6, usocket.SOCK_DGRAM)

    def log(self, msg):
        self._socket.sendto(msg, self._addr)

class PrintLogger():
    def log(self, msg):
        print(msg)


def createFormatter():
    def micropythonFormatter(msg):
        now = utime.localtime()
        return "[%d-%02d-%02d %02d:%02d:%02d] %s" % (now[0], now[1], now[2], now[3], now[4], now[5], msg)

    # def pythonFormatter(msg):
    #     return msg
    import utime

    return micropythonFormatter

_logger = None
_formatter = createFormatter()

def log(msg):
    if _logger == None: return
    _logger.log(_formatter(msg) + '\n')

def setupUDPLogger(host, port):
    global _logger
    _logger = UDPLogger(host, port)

def setupPrintLogger():
    global _logger
    _logger = PrintLogger()

def disable():
    _logger = None

setupUDPLogger("localhost", 9000)
# setupPrintLogger()

log("Hello world")