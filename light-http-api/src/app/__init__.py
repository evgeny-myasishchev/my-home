import uhttp
import middleware
import logger
import ure
import ujson

from . import config
from . import controller
from . import routes
from . import setup

def create_server(config):
    light_controller = controller.create_light_controller(config['light-controller'])

    router = middleware.create_router([
        ('GET', '/v1/light/ping', routes.create_ping_handler(light_controller)),
        ('POST', '/v1/light/led', routes.create_led_handler(light_controller)),

        ('GET', ure.compile(r'^/v1/light/pins/(\d+)$'), routes.create_get_pin_handler(light_controller)),
        ('POST', ure.compile(r'^/v1/light/pins/(\d+)$'), routes.create_set_pin_handler(light_controller))
    ])

    default_handler = middleware.not_found(None)
    server = uhttp.HTTPServer(
        handler=middleware.use(default_handler,
            middleware.trace,
            middleware.recover,
            router,
        ),
        port=config['server']['port'],
    )
    return server