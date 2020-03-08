import app
import logger

config = app.config.load()
logger.setup(transport=app.setup.logger_transport(config['log']))

server = app.create_server(config)

def startServer():
    try:
        server.start()
    except KeyboardInterrupt as err:
        server.stop()
    except BaseException as err:
        logger.error("Failed to start server\n", err=err)

if config["server"]["enabled"] == True:
    startServer()