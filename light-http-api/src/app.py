import uhttp
import middleware
import logger

def pong(w, req):
    w.write("PONG")

def main():
    logger.setup()
    server = uhttp.HTTPServer(
        handler=middleware.use(pong,
            middleware.recover,
            middleware.trace,
        ),
        port=8080,
    )
    try:
        server.start()
    except KeyboardInterrupt:
        server.stop()



if __name__ == '__main__':
    main()