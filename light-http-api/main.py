import app
import logger
import uio
import sys

transport = logger.make_combined_transport(
    logger.make_udp_transport('10.1.0.19', 50001),
    logger.make_file_transport('application.log')
)

logger.setup(transport=transport)

server = None
try:
    server = app.start_server()
except KeyboardInterrupt as err:
    logger.error("Got keyboard interrupt", err=err)
    server.stop()
except BaseException as err:
    logger.error("Failed to start server\n", err=err)