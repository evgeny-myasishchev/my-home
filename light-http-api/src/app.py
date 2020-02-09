import uhttp
import middleware
import logger
import ure

def pong(w, req):
    w.write("PONG")

def get_pin(w, req, match):
    w.write("PIN: %s" % match.group(1))

def start_server(*, port=8080):
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
    return server