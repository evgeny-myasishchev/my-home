import uhttp
import middleware
import logger
import ure

def pong(w, req):
    w.write("PONG")

def get_pin(w, req, match):
    w.write("PIN: %s" % match.group(1))

def start_server(*, 
    logger_transport=logger.print_transport,
    port=8080,
):
    logger.setup(transport=logger_transport)

    router = middleware.create_router([
        ("/ping", pong),
        (ure.compile(r"^/pins/(\d+)$"), get_pin)
    ])

    default_handler = middleware.not_found(None)
    server = uhttp.HTTPServer(
        handler=middleware.use(default_handler,
            middleware.recover,
            middleware.trace,
            router,
        ),
        port=port,
    )
    server.start()