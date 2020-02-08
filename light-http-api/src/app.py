import uhttp
import middleware
import logger

def pong(w, req):
    w.write("PONG")

def main():
    logger.setup()

    router = middleware.create_router([
        ("/ping", pong)
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