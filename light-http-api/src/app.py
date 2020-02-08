import uhttp
import middleware
import logger
import ure

def pong(w, req):
    w.write("PONG")

def get_pin(w, req, match):
    w.write("PIN: %s" % match.group(1))

def main():
    logger.setup()

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
        port=8080,
    )
    try:
        server.start()
    except KeyboardInterrupt:
        server.stop()

if __name__ == '__main__':
    main()