import uhttp
import middleware
import logger
import ure
import ujson
# from machine import UART
from . import config
from . import setup
from . import routes

# uart = UART(0)

# def pong(w, req):
#     w.write("PONG")

# def led_on(w, req):
#     uart.write("AT+LED=ON\n")
#     w.write("LED_ON")

# def led_off(w, req):
#     uart.write("AT+LED=OFF\n")
#     w.write("LED_OFF")

# def get_pin(w, req, match):
#     w.write("PIN: %s" % match.group(1))

# def test_post(w, req):
#     body = ujson.load(req.body())
#     print(body)

def create_server(*, port=8080):
    router = middleware.create_router([
        ('GET', '/v1/ping', routes.create_ping_handler()),
        # ('POST', '/test-post', test_post),
        # ("/led/on", led_on),
        # ("/led/off", led_off),
        # ('GET', ure.compile(r"^/pins/(\d+)$"), get_pin)
    ])

    default_handler = middleware.not_found(None)
    server = uhttp.HTTPServer(
        handler=middleware.use(default_handler,
            middleware.trace,
            middleware.recover,
            router,
        ),
        port=port,
    )
    return server